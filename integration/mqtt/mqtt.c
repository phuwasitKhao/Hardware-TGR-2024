#include "../main_thr.h"
#include <string.h>
#include <MQTTClient.h>
#include <pthread.h>
#include <unistd.h>  // For sleep()

// Private constants
const char base_topic[] = "topgun/project/%s";
const char MQTT_BROKER[] = "tcp://185.84.161.188:1884";
const char MQTT_CLIENTID[] = "pi_5";

// Assuming `shared_message`, `data_cond`, `data_cond_mutex` etc. are declared globally
extern pthread_mutex_t data_cond_mutex;
extern pthread_cond_t data_cond;
extern pthread_mutex_t db_dump_cond_mutex;
extern pthread_cond_t db_dump_cond;
extern char shared_message[256];
extern char shared_db_dump[256];

int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    char message_d[256];

    // Safely copy the message payload to `message_d`
    strncpy(message_d, (char *)message->payload, sizeof(message_d) - 1);
    message_d[sizeof(message_d) - 1] = '\0';

    // Lock mutex, update shared_message, then broadcast to waiting threads
    pthread_mutex_lock(&data_cond_mutex);
    printf("Message arrived on topic %s: %s\n", topicName, message_d);
    if (strcmp(topicName, "topgun/project/db_dump") == 0)
    {
        strncpy(shared_db_dump, message_d, sizeof(shared_db_dump) - 1);
        shared_db_dump[sizeof(shared_db_dump) - 1] = '\0';
        pthread_cond_broadcast(&db_dump_cond);
        pthread_mutex_unlock(&db_dump_cond_mutex);
        return 1;
    } else if (strcmp(topicName, "topgun/project/hw") == 0)
    {
        strncpy(shared_message, message_d, sizeof(shared_message) - 1);
        shared_message[sizeof(shared_message) - 1] = '\0';
        pthread_cond_broadcast(&data_cond);
        pthread_mutex_unlock(&data_cond_mutex);
    } else if (strcmp(topicName, "topgun/project/sw") == 0)
    {
        /* code */
    } else if (strcmp(topicName, "topgun/project/project") == 0)
    {
        /* code */
    }
    
    
    strncpy(shared_message, message_d, sizeof(shared_message) - 1);
    shared_message[sizeof(shared_message) - 1] = '\0';
    pthread_cond_broadcast(&data_cond);
    pthread_mutex_unlock(&data_cond_mutex);

    // Free MQTT message resources
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    return 1;
}

void *mqtt_thr_fcn(char *ptr[256])
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

    if ((rc = MQTTClient_connect(mqtt_client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    snprintf(topic, sizeof(topic), base_topic, "db_dump");
    printf("MQTT TOPIC: %s\n", topic);

    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    while (1) {
        char db_name[256];

        // Wait for condition to be met and retrieve shared data
        pthread_mutex_lock(&db_dump_cond_mutex);
        pthread_cond_wait(&db_dump_cond, &db_dump_cond_mutex);
        printf("Shared data: %s\n", shared_db_dump);
        strncpy(db_name, shared_db_dump, sizeof(db_name) - 1);
        db_name[sizeof(db_name) - 1] = '\0';
        pthread_mutex_unlock(&db_dump_cond_mutex);

        // Query the database or perform other actions
        int mem_free = get_mem_free();
        cJSON *json = cJSON_CreateObject();
        cJSON_AddStringToObject(json, "db_name", db_name);
        cJSON_AddNumberToObject(json, "value", mem_free);

        char *json_payload = cJSON_Print(json);
        pubmsg.payload = json_payload;
        pubmsg.payloadlen = strlen(json_payload);
        pubmsg.qos = 0;
        pubmsg.retained = 0;

        if ((rc = MQTTClient_publishMessage(mqtt_client, topic, &pubmsg, &token)) == MQTTCLIENT_SUCCESS) {
            printf("Published to %s: %s\n", topic, json_payload);
        } else {
            printf("Failed to publish message, return code %d\n", rc);
        }

        // Free JSON objects and payload
        cJSON_Delete(json);
        free(json_payload);

        sleep(1);  // Add sleep to reduce CPU usage in the loop
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

    MQTTClient_create(&mqtt_client, MQTT_BROKER, MQTT_CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = "changerzaryx";
    conn_opts.password = "cn16022547";
    MQTTClient_setCallbacks(mqtt_client, NULL, NULL, messageArrived, NULL);

    if ((rc = MQTTClient_connect(mqtt_client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    snprintf(topic, sizeof(topic), base_topic, "#");  // Subscribe to all subtopics under base_topic
    if ((rc = MQTTClient_subscribe(mqtt_client, topic, 0)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to subscribe, return code %d\n", rc);
        MQTTClient_disconnect(mqtt_client, 10000);
        MQTTClient_destroy(&mqtt_client);
        return NULL;
    }
    printf("Subscribed to topic: %s\n", topic);

    // Keep the subscriber running
    while (1) {
        sleep(1);  // Sleep to allow messageArrived to handle messages
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

    if ((fh = fopen("/proc/meminfo", "r")) == NULL) {
        perror("Error opening /proc/meminfo");
        return -1;
    }

    fread(buf, 1, sizeof(buf) - 1, fh);
    fclose(fh);
    buf[sizeof(buf) - 1] = '\0';

    tmp_buf = strtok(buf, "\n ");
    while (tmp_buf) {
        if (strcmp(tmp_buf, "MemFree:") == 0) {
            tmp_buf = strtok(NULL, "\n ");
            if (tmp_buf) {
                mem_free_size = atoi(tmp_buf);
                break;
            }
        }
        tmp_buf = strtok(NULL, "\n ");
    }

    printf("Memory free: %d\n", mem_free_size);
    return mem_free_size;
}
