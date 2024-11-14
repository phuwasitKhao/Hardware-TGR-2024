#include "../main_thr.h"
#include <curl/curl.h>
#include "rest.c"



void *rest_thr_fcn(void *ptr)
{
    printf("Starting REST thread\n");
    char message[256];
    while (1)
    {
        // Wait for the shared message to be updated
        pthread_mutex_lock(&data_cond_mutex);
        pthread_cond_wait(&data_cond, &data_cond_mutex);
        strcpy(message, shared_message);
        pthread_mutex_unlock(&data_cond_mutex);

        // Check if the message is "exit"
        if (strcmp(message, ""))

        // Print the message
        printf("REST: %s\n", message);
    }
    
}