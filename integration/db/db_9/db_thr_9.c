#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "db_helper.h"        // Include your database helper functions
#include "../../main_thr.h"   // Include main_thr.h for shared resources

// Declaration of shared resources
extern pthread_mutex_t data_cond_mutex;   // Mutex for condition variable
extern pthread_cond_t data_cond;          // Condition variable to signal new data
extern char shared_message[256];          // Shared message buffer

// Thread function to handle database logging
void *db_9_cmd_thr(char *ptr[]) {
    printf("Starting database SQLite thread\n");

    char db_name[] = "logs_9.db";

    // Initialize the database and create the logs table if it doesn't exist
    if (dbase_init(db_name, "CREATE TABLE IF NOT EXISTS logs (\
                          id INTEGER PRIMARY KEY AUTOINCREMENT, \
                          details VARCHAR(255), \
                          timestamp DATETIME DEFAULT (DATETIME('now', 'localtime')))")) {
        fprintf(stderr, "Failed to initialize database: %s\n", db_name);
        return NULL;
    }

    char mes[256];
    while (1) {
        // Wait for data to be updated in shared_message
        pthread_mutex_lock(&data_cond_mutex);
        pthread_cond_wait(&data_cond, &data_cond_mutex);

        // Copy shared_message safely into mes and unlock the mutex
        strncpy(mes, shared_message, sizeof(mes) - 1);
        mes[sizeof(mes) - 1] = '\0';  // Ensure null-termination
        pthread_mutex_unlock(&data_cond_mutex);

        printf("Received message: %s\n", mes);

        // Append the message to the database
        if (dbase_append(db_name, "logs", mes) != 0) {
            fprintf(stderr, "Failed to append message to database: %s\n", mes);
        }
        
    }

    return NULL;  // Proper return for a pthread function with `void *` return type
}
