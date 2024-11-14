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
extern int shared_value;
extern char shared_message[256]; // Store the shared message here

// function prototypes
// void *rest_thr_fcn( void *ptr );
void *message_print(void *ptr);
void *mqtt_thr_fcn( void *ptr);
void *mqtt_thr_fcn_sub( void *ptr);
void *mid_thr_fcn( void *ptr );
void *end_thr_fcn(void *ptr);
void *rest_thr_fcn(void *ptr);
#endif // IOT_APP_H