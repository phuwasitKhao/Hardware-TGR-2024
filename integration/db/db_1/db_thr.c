#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "db_helper.h"      // Include your database helper functions
#include "../../main_thr.h" // Include main_thr.h for shared resources

// Declaration of shared resources
extern pthread_mutex_t data_cond_mutex; // Mutex for condition variable
extern pthread_cond_t data_cond;        // Condition variable to signal new data
extern char shared_message[256];        // Shared message buffer
#define DB_NAME "logs.db"

// Thread function to handle database logging
void *db_1_cmd_thr(char *ptr[])
{
    printf("Starting database SQLite thread\n");

    // Initialize the database and create the logs table if it doesn't exist
    if (dbase_init)
    {
        fprintf(stderr, "Failed to initialize database: %s\n", DB_NAME);
        return NULL;
    }

    char mes[256];
    while (1)
    {
        // Wait for data to be updated in shared_message
        pthread_mutex_lock(&data_cond_mutex);
        pthread_cond_wait(&data_cond, &data_cond_mutex);

        // Copy shared_message safely into mes and unlock the mutex
        strncpy(mes, shared_message, sizeof(mes) - 1);
        mes[sizeof(mes) - 1] = '\0'; // Ensure null-termination
        pthread_mutex_unlock(&data_cond_mutex);

        printf("Received message: %s\n", mes);

    }

    return NULL; // Proper return for a pthread function with `void *` return type
}
