#include "main.h"

pthread_mutex_t data_cond_mutex;
pthread_cond_t data_cond;
int shared_value = 0;
char shared_message[256];

int main(int argc, char *argv[]) {
    pthread_t mqtt_sub_thr, print_thr, mid_thr, db_thr;

    // Check if the required arguments are passed
    // if (argc < ) {
    //     printf("Usage: %s <REST_ENDPOINT> <MQTT_PUBLISH_TOPIC> <MQTT_SUBSCRIBE_TOPIC>\n", argv[0]);
    //     exit(1);
    // }

    // Initialize IPC objects
    pthread_mutex_init(&data_cond_mutex, NULL);
    pthread_cond_init(&data_cond, NULL);

    // Create threads
    // Uncomment the following line to create the REST thread if necessary
    // pthread_create(&rest_thr, NULL, rest_thr_fcn, (void*)argv[1]);
    // pthread_create(&mqtt_pub_thr, NULL, mqtt_thr_fcn, (void*)argv[1]);
    // pthread_create(&print_thr, NULL, message_print, (void*)argv[1]);
    pthread_create(&mqtt_sub_thr, NULL, mqtt_thr_fcn_sub, (void*)argv[2]);
    pthread_create(&mid_thr, NULL, mid_thr_fcn, (void*)argv[2]);
    pthread_create(&db_thr, NULL, end_thr_fcn, (void*)argv[2]);
    // Wait for all threads to terminate
    // Uncomment the following line to wait for the REST thread if necessary
    // pthread_join(rest_thr, NULL);
    pthread_join(mid_thr, NULL);
    pthread_join(mqtt_sub_thr, NULL);  // Wait for MQTT subscribing thread
    pthread_join(db_thr,NULL);
    // pthread_join(print_thr, NULL);     // Wait for message printing thread

    // Clean up IPC objects
    pthread_mutex_destroy(&data_cond_mutex);
    pthread_cond_destroy(&data_cond);

    return 0;
}
