#include "iot_app.h"
#include <string.h>

void *message_print(void *ptr)
{   
    printf("Starting print thread\n");

    char mes[256];
    while (1)
    {
        pthread_mutex_lock(&data_cond_mutex);
        
        pthread_cond_wait(&data_cond, &data_cond_mutex);
        
        strncpy(mes, shared_message, sizeof(mes) - 1);
        mes[sizeof(mes) - 1] = '\0';  // Ensure null-termination
        
        printf("%s\n", mes);
        
        pthread_mutex_unlock(&data_cond_mutex);
    }
    
    return NULL;  // Always return a value from the thread function
}