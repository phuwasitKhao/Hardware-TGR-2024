#ifndef IOT_APP_H
#define IOT_APP_H

// include files

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <cjson/cJSON.h>

// shared variables
extern pthread_mutex_t data_cond_mutex;
extern pthread_cond_t data_cond;
extern pthread_mutex_t db_dump_cond_mutex;
extern pthread_cond_t db_dump_cond;
extern pthread_mutex_t audio_cond_mutex;
extern pthread_cond_t audio_cond;
extern pthread_mutex_t rest_cond_mutex;
extern pthread_cond_t rest_cond;
extern pthread_mutex_t predict_mutex;
extern pthread_cond_t predict_cond;

extern int shared_value;
extern char shared_message[256];         // Store the shared message here
extern char shared_db_dump[256];         // Store the shared db_dump here
extern char shared_message_hw[256];      // Store the shared message here
extern char shared_message_sw[256];      // Store the shared message here
extern char shared_message_project[256]; // Store the shared message here
extern char shared_name_ouput_file[256];
extern int shared_predict;
// extern int shared_detect = 0;
extern short shared_buf[];

// extern const char INIT_SQL_CMD[];
// extern const char APPEND_SQL_CMD[];
// extern const char QUERY_SQL_CMD[];

// function prototypes
// void *rest_thr_fcn( void *ptr );
void *message_print(void *ptr);
void *mqtt_thr_fcn(char *ptr[]);
void *mqtt_thr_fcn_sub(void *ptr);
void *db_9_cmd_thr(char *ptr[]);
void *db_1_cmd_thr(char *ptr[]);
void *write_buff_thr(char *ptr[]);
void *alsa_thr_fcn(void *ptr);
void *fft_thr_fcn(void *ptr);
void *rest_thr_fcn(void *ptr);
void *mqtt_thr_fcn_freq(char *ptr[256]);
#endif // IOT_APP_H