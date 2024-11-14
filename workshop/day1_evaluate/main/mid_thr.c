#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Define the mid thread function
void *mid_thr_fcn(void *ptr)
{
    printf("Starting mid thread\n");
    char json_str[256];
    char mes[256];
    int mem_total, mem_free, mem_available;

    while (1)
    {
        pthread_mutex_lock(&data_cond_mutex);

        pthread_cond_wait(&data_cond, &data_cond_mutex);

        // Get the shared message and ensure it is null-terminated
        strncpy(json_str, shared_message, sizeof(json_str) - 1);
        json_str[sizeof(json_str) - 1] = '\0';

        // printf("Received message: %s\n", json_str);

        // Compare the received message with the expected strings
        cJSON *json = cJSON_Parse(json_str);
        if (json == NULL)
        {
            printf("Error parsing JSON\n");
            pthread_mutex_unlock(&data_cond_mutex);
            continue;
        }
        get_memory_info(&mem_total, &mem_free, &mem_available);

        cJSON *check_item = cJSON_GetObjectItemCaseSensitive(json, "check");
        if (cJSON_IsString(check_item) && check_item->valuestring != NULL)
        {
            // Check what to report based on the "check" value
            if (strcmp(check_item->valuestring, "MemTotal") == 0)
            {
                printf("MemTotal: %d kB\n", mem_total);
                shared_value = mem_total;
            }
            else if (strcmp(check_item->valuestring, "MemFree") == 0)
            {
                printf("MemFree: %d kB\n", mem_free);
                shared_value = mem_free;
            }
            else if (strcmp(check_item->valuestring, "MemAvailable") == 0)
            {
                printf("MemAvailable: %d kB\n", mem_available);
                shared_value = mem_available;
            }
            else
            {
                printf("Error: Unknown check value received: %s\n", check_item->valuestring);
            }
        }
        pthread_cond_broadcast(&data_cond);
        pthread_mutex_unlock(&data_cond_mutex);
    }

    return NULL; // Always return a value from the thread function
}

void get_memory_info(int *mem_total, int *mem_free, int *mem_available)
{
    FILE *fh;
    char buf[256];

    *mem_total = 0;
    *mem_free = 0;
    *mem_available = 0;

    fh = fopen("/proc/meminfo", "r");
    if (fh == NULL)
    {
        perror("Failed to open /proc/meminfo");
        return;
    }
    
    while (fgets(buf, sizeof(buf), fh))
    {
        if (strncmp(buf, "MemTotal:", 9) == 0)
        {
            sscanf(buf, "MemTotal: %d kB", mem_total);
            // printf(mem_total);
        }
        else if (strncmp(buf, "MemFree:", 8) == 0)
        {
            sscanf(buf, "MemFree: %d kB", mem_free);
            // printf(mem_free);
        }
        else if (strncmp(buf, "MemAvailable:", 13) == 0)
        {
            sscanf(buf, "MemAvailable: %d kB", mem_available);
            // printf(mem_total);
        }
    }

    fclose(fh);
}
