#include "iot_app.h"
#include <string.h>
#include <MQTTClient.h>

// private constants
const char base_topic[] = "tgr2024/team/bb-see-it-kku";
const char MQTT_BROKER[] = "tcp://broker.emqx.io:1883";
const char MQTT_CLIENTID[] = "pi_5";

int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    char message_d[256];
    
    // Log the arrival of the message
    // printf("Message arrived on topic %s: %s\n", topicName, (char *)message->payload);
    
    // Safely copy the message to 'message_d', ensuring null-termination
    strncpy(message_d, (char *)message->payload, sizeof(message_d) - 1);
    message_d[sizeof(message_d) - 1] = '\0';  // Null-terminate the string

    // Lock the mutex to safely update the shared_message
    pthread_mutex_lock(&data_cond_mutex);
    
    // Safely copy the message to the shared_message
    strncpy(shared_message, message_d, sizeof(shared_message) - 1);
    shared_message[sizeof(shared_message) - 1] = '\0'; // Ensure null-termination
    
    pthread_cond_broadcast(&data_cond); // Notify all waiting threads
    pthread_mutex_unlock(&data_cond_mutex);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    return 1;
}




void *mqtt_thr_fcn(void *ptr)
{
    // setup
    char topic[100];
    int rc;

    printf("Starting MQTT thread\n");
    MQTTClient mqtt_client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_deliveryToken token;
    MQTTClient_create(&mqtt_client, MQTT_BROKER, MQTT_CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTClient_connect(mqtt_client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    sprintf(topic, base_topic, (char *)ptr);
    printf("MQTT TOPIC: %s\n", topic);
    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    while (1)
    {
        // loop
        float value;

        pthread_mutex_lock(&data_cond_mutex);
        pthread_cond_wait(&data_cond, &data_cond_mutex);
        printf("Shared data: %d\n", shared_value);
        value = shared_value;
        pthread_mutex_unlock(&data_cond_mutex);
        cJSON *json = cJSON_CreateObject();
        cJSON_AddStringToObject(json, "team", (char *)ptr);
        cJSON_AddNumberToObject(json, "value", value);
        char *json_payload = cJSON_Print(json);
        pubmsg.payload = json_payload;
        pubmsg.payloadlen = strlen(json_payload);
        pubmsg.qos = 0;
        pubmsg.retained = 0;
        MQTTClient_publishMessage(mqtt_client, topic, &pubmsg, &token);
        printf("Publish to %s\n", topic);
        cJSON_free(json);
    }
    MQTTClient_destroy(mqtt_client);
}




void *mqtt_thr_fcn_sub(void *ptr) {
    // MQTT client setup
    printf("tart thred MQTT sub");
    int rc;
    char topic[100];

    printf("Starting MQTT Subscriber thread\n");
    MQTTClient mqtt_client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_create(&mqtt_client, MQTT_BROKER, MQTT_CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    MQTTClient_setCallbacks(mqtt_client, NULL, NULL, messageArrived, NULL);

    if ((rc = MQTTClient_connect(mqtt_client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    // Subscribe to the topic
    sprintf(topic, "%s/#", base_topic); // Subscribe to all sub-topics under base_topic
    if ((rc = MQTTClient_subscribe(mqtt_client, topic, 0)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to subscribe, return code %d\n", rc);
        MQTTClient_disconnect(mqtt_client, 10000);
        MQTTClient_destroy(&mqtt_client);
        return NULL;
    }
    printf("Subscribed to topic: %s\n", topic);

    // Keep the subscriber running to listen for incoming messages
    while (1) {
        // Wait here; messageArrived handles incoming messages.
        sleep(1);
    }

    // Cleanup (not reached in this example, but good practice)
    MQTTClient_disconnect(mqtt_client, 10000);
    MQTTClient_destroy(&mqtt_client);
    return NULL;
}