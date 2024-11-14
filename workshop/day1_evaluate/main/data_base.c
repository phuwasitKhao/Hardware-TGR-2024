#include "main.h"
#include "db_helper.h"
#include <string.h>
#include <cjson/cJSON.h>
// private constants
// const char INIT_SQL_CMD[] = "CREATE TABLE IF NOT EXISTS data_table (\
//     _id INTEGER PRIMARY KEY AUTOINCREMENT, \
//     timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,\
//     value INTEGER \
// )";
// private variables

// private functions

void *end_thr_fcn(void *ptr)
{
    printf("Start Thred DB");
    char db_name[] = "cmd.db";
    dbase_init(db_name);
    sqlite3_stmt *stmt;
    int mem = 0;
    char mes[256];
    time_t now;
    struct tm *timeinfo;
    // setup
    time(&now);
    timeinfo = localtime(&now);
    char *timestamp = asctime(timeinfo);

    while (1)
    {
        printf("Thread end starts at: %s", timestamp);
        pthread_mutex_lock(&data_cond_mutex);
        pthread_cond_wait(&data_cond, &data_cond_mutex);

        strncpy(mes, shared_message, sizeof(mes) - 1);
        mes[sizeof(mes) - 1] = '\0';

        mem = shared_value;
        dbase_append(db_name, mes, mem, timestamp);
        pthread_mutex_unlock(&data_cond_mutex);
    }
}