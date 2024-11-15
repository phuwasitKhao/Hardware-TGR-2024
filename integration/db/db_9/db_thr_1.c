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

// Thread function to handle database logging
void *db_1_cmd_thr(char *ptr[])
{
    printf("Starting database SQLite thread\n");

    char db_name[] = "logs_1.db";

    // Initialize the database and create the logs table if it doesn't exist
    if (dbase_init(db_name, "CREATE TABLE IF NOT EXISTS logs (\n"
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, \n"
                            "details TEXT, \n"
                            "timestamp DATETIME DEFAULT (DATETIME('now', 'localtime'))\n)"))
    {
        fprintf(stderr, "Failed to initialize database: %s\n", db_name);
        return NULL;
    }

    char mes[256];
    int file_size = 0; // ตัวแปรเก็บขนาดไฟล์
    while (1)
    {
        // Wait for data to be updated in shared_message
        pthread_mutex_lock(&data_cond_mutex);
        pthread_cond_wait(&data_cond, &data_cond_mutex);

        // Copy shared_message safely into mes and unlock the mutex
        strncpy(mes, shared_message, sizeof(mes) - 1);
        mes[sizeof(mes) - 1] = '\0'; // Ensure null-termination
        file_size = strlen(mes);     // กำหนดขนาดไฟล์เป็นขนาดของข้อความ
        pthread_mutex_unlock(&data_cond_mutex);

        printf("Received message: %s\n", mes);

        // Append the message and file size to the database
        if (dbase_append_with_size(db_name, "logs", mes, file_size) != 0)
        {
            fprintf(stderr, "Failed to append message to database: %s\n", mes);
        }
        else
        {
            printf("Successfully logged file: %s with size: %d\n", mes, file_size);
        }
    }

    return NULL;
}
