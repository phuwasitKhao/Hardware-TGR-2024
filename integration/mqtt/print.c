#include "../main_thr.h"
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
        pthread_mutex_unlock(&data_cond_mutex);
        printf("%s\n", mes);
        
        if (strcmp(mes, "00") == 0)
        {
            printf("Start Post file soud to db\n");
        }else if (strcmp(mes, "01") == 0)
        {
            printf("Start Download mode and insatll\n");
        }else if (strcmp(mes, "10") == 0)
        {
            printf("Start Dump data base\nl");
           pthread_mutex_lock(&db_dump_cond_mutex);
            pthread_cond_signal(&db_dump_cond);
            pthread_mutex_unlock(&db_dump_cond_mutex);

        }else {
            printf("not code!!\n");
        }

        
    }
    
    return NULL;  // Always return a value from the thread function
}
