#include "../main_thr.h"
#include <stdio.h>
#include <MQTTClient.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>      // For sleep()
#include <cjson/cJSON.h> // Ensure cJSON is included for JSON handling

// Private constants
const char base_topic[] = "topgun/project/%s";
const char MQTT_BROKER[] = "tcp://185.84.161.188:1884";
const char MQTT_CLIENTID[] = "pi_5";
const char MQTT_CLIENTID_sub[] = "pi_5_sub";

// Assuming `shared_message`, `data_cond`, `data_cond_mutex` etc. are declared globally
// extern pthread_mutex_t data_cond_mutex;
// extern pthread_cond_t data_cond;
// extern pthread_mutex_t db_dump_cond_mutex;
// extern pthread_cond_t db_dump_cond;
// extern char shared_db_dump[256];

int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    char message_d[256];

    // Safely copy the message payload to `message_d`
    strncpy(message_d, (char *)message->payload, sizeof(message_d) - 1);
    message_d[sizeof(message_d) - 1] = '\0';

    // Lock mutex, update shared_message, then broadcast to waiting threads
    pthread_mutex_lock(&data_cond_mutex);
    printf("Message arrived on topic %s: %s\n", topicName, message_d);
    strncpy(shared_message, message_d, sizeof(shared_message) - 1);
    shared_message[sizeof(shared_message) - 1] = '\0';
    pthread_cond_signal(&data_cond);
    pthread_mutex_unlock(&data_cond_mutex);
    pthread_mutex_lock(&rest_cond_mutex);
    strncpy(shared_message_sw, message_d, sizeof(shared_message) - 1);
    shared_message_sw[sizeof(shared_message_sw) - 1] = '\0';
    pthread_cond_signal(&rest_cond);
    pthread_mutex_unlock(&rest_cond_mutex);
    // Free MQTT message resources
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    return 1;
}
// Function to publish prediction results
void *mqtt_thr_fcn(char *ptr[256])
{
    printf("Starting MQTT prediction publishing thread\n");

    // Setup MQTT client
    int rc;
    MQTTClient mqtt_client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_deliveryToken token;

    MQTTClient_create(&mqtt_client, MQTT_BROKER, MQTT_CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = "changerzaryx";
    conn_opts.password = "cn16022547";

    if ((rc = MQTTClient_connect(mqtt_client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect to MQTT broker, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    printf("Connected to MQTT broker, publishing to topic: predict\n");

    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    while (1)
    {
        // Wait for prediction result update
        pthread_mutex_lock(&predict_mutex);
        pthread_cond_wait(&predict_cond, &predict_mutex);

        // Capture the prediction result
        int prediction = 0;
        prediction = shared_predict;   // Use shared variable for prediction

        pthread_mutex_unlock(&predict_mutex);

        // Get the current timestamp
        time_t now = time(NULL);
        struct tm *timeinfo = localtime(&now);
        char timestamp[64];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H:%M:%S", timeinfo);


        // Format prediction data into JSON
        cJSON *json = cJSON_CreateObject();
        cJSON_AddStringToObject(json, "time", timestamp);
        cJSON_AddNumberToObject(json, "prediction", prediction);

        char *json_payload = cJSON_PrintUnformatted(json);
        pubmsg.payload = json_payload;
        pubmsg.payloadlen = strlen(json_payload);
        pubmsg.qos = 0;
        pubmsg.retained = 0;

        // Publish to MQTT topic
        if ((rc = MQTTClient_publishMessage(mqtt_client, "topgun/predict_kku", &pubmsg, &token)) == MQTTCLIENT_SUCCESS)
        {
            printf("Published to topic: topgun/predict_kku, Message: %s\n", json_payload);
        }
        else
        {
            printf("Failed to publish message, return code %d\n", rc);
        }

        // Free JSON objects and payload
        cJSON_Delete(json);
        free(json_payload);

        // Sleep to control publishing frequency (if needed)
        usleep(100000); // Sleep for 100ms to reduce CPU usage
    }

    // Disconnect and clean up MQTT client
    MQTTClient_disconnect(mqtt_client, 10000);
    MQTTClient_destroy(&mqtt_client);

    return NULL;
}

void *mqtt_thr_fcn_freq(char *ptr[256])
{
    printf("Starting MQTT thread\n");

    // Setup MQTT client
    char topic[100];
    int rc;
    MQTTClient mqtt_client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_deliveryToken token;

    MQTTClient_create(&mqtt_client, MQTT_BROKER, MQTT_CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = "changerzaryx";
    conn_opts.password = "cn16022547";

    if ((rc = MQTTClient_connect(mqtt_client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    snprintf(topic, sizeof(topic), "topgun/data"); // Set topic to 'topgun/data'
    printf("MQTT TOPIC: %s\n", topic);

    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    while (1)
    {
        // Lock, copy data to a local buffer, and unlock
        pthread_mutex_lock(&audio_cond_mutex);
        short buffer_copy[2048];
        memcpy(buffer_copy, shared_buf, sizeof(buffer_copy));
        pthread_mutex_unlock(&audio_cond_mutex);

        // Convert buffer to JSON format
        cJSON *json = cJSON_CreateObject();
        cJSON *data_array = cJSON_CreateIntArray(buffer_copy, 2048); // Serialize buffer

        cJSON_AddItemToObject(json, "data", data_array);

        char *json_payload = cJSON_PrintUnformatted(json);
        pubmsg.payload = json_payload;
        pubmsg.payloadlen = strlen(json_payload);
        pubmsg.qos = 0;
        pubmsg.retained = 0;

        // Publish to MQTT
        if ((rc = MQTTClient_publishMessage(mqtt_client, topic, &pubmsg, &token)) == MQTTCLIENT_SUCCESS)
        {
            printf("Published to %s\n", topic);
        }
        else
        {
            printf("Failed to publish message, return code %d\n", rc);
        }

        // Free JSON objects and payload
        cJSON_Delete(json);
        free(json_payload);

        sleep(1); // Adjust sleep time as needed for update rate
    }

    MQTTClient_disconnect(mqtt_client, 10000);
    MQTTClient_destroy(&mqtt_client);
    return NULL;
}

void *mqtt_thr_fcn_sub(void *ptr)
{
    printf("Starting MQTT Subscriber thread\n");

    // Setup MQTT subscriber
    int rc;
    char topic[100];
    MQTTClient mqtt_client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    MQTTClient_create(&mqtt_client, MQTT_BROKER, MQTT_CLIENTID_sub, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = "changerzaryx";
    conn_opts.password = "cn16022547";
    MQTTClient_setCallbacks(mqtt_client, NULL, NULL, messageArrived, NULL);

    if ((rc = MQTTClient_connect(mqtt_client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    snprintf(topic, sizeof(topic), base_topic, "#"); // Subscribe to all subtopics under base_topic
    if ((rc = MQTTClient_subscribe(mqtt_client, topic, 0)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to subscribe, return code %d\n", rc);
        MQTTClient_disconnect(mqtt_client, 10000);
        MQTTClient_destroy(&mqtt_client);
        return NULL;
    }
    printf("Subscribed to topic: %s\n", topic);

    // Keep the subscriber running
    while (1)
    {
        sleep(1); // Sleep to allow messageArrived to handle messages
    }

    // Cleanup
    MQTTClient_disconnect(mqtt_client, 10000);
    MQTTClient_destroy(&mqtt_client);
    return NULL;
}

int get_mem_free()
{
    FILE *fh;
    char buf[5000];
    char *tmp_buf;
    int mem_free_size = 0;

    if ((fh = fopen("/proc/meminfo", "r")) == NULL)
    {
        perror("Error opening /proc/meminfo");
        return -1;
    }

    fread(buf, 1, sizeof(buf) - 1, fh);
    fclose(fh);
    buf[sizeof(buf) - 1] = '\0';

    tmp_buf = strtok(buf, "\n ");
    while (tmp_buf)
    {
        if (strcmp(tmp_buf, "MemFree:") == 0)
        {
            tmp_buf = strtok(NULL, "\n ");
            if (tmp_buf)
            {
                mem_free_size = atoi(tmp_buf);
                break;
            }
        }
        tmp_buf = strtok(NULL, "\n ");
    }

    printf("Memory free: %d\n", mem_free_size);
    return mem_free_size;
}

void check_database(const char *message)
{
    if (strcmp(message, "db01") == 0)
    {
        message = "db_01.db";
    }
    else if (strcmp(message, "db02") == 0)
    {
        message = "db_02.db";
    }
    else if (strcmp(message, "db06") == 0)
    {
        message = "db_06.db";
    }
    else if (strcmp(message, "db09") == 0)
    {
        message = "db_09.db";
    }
    else
    {
        printf("Message does not match any known database\n");
        return NULL; // Return NULL to indicate no match
    }
}