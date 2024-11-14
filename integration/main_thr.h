#ifndef IOT_APP_H
#define IOT_APP_H

// include files
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <cjson/cJSON.h>


// shared variables
extern pthread_mutex_t data_cond_mutex;
extern pthread_cond_t data_cond;
extern pthread_mutex_t db_dump_cond_mutex;
extern pthread_cond_t db_dump_cond;
extern int shared_value;
extern char shared_message[256]; // Store the shared message here
extern char shared_db_dump[256]; // Store the shared db_dump here
extern char shared_message_hw[256]; // Store the shared message here
extern char shared_message_sw[256]; // Store the shared message here    
extern char shared_message_project[256]; // Store the shared message here

extern short shared_buf[];


// extern const char INIT_SQL_CMD[];
// extern const char APPEND_SQL_CMD[];
// extern const char QUERY_SQL_CMD[];


// function prototypes
// void *rest_thr_fcn( void *ptr );
void *message_print(void *ptr);
void *mqtt_thr_fcn( char *ptr[]);
void *mqtt_thr_fcn_sub( void *ptr);
void *db_9_cmd_thr( char *ptr[]);
void *write_buff_thr( char *ptr[]);
void *alsa_thr_fcn( void *ptr );
void *fft_thr_fcn( void *ptr );

#endif // IOT_APP_H