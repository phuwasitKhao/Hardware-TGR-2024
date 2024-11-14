#include "main_thr.h"
#include "db/db_9/db_helper.h"
pthread_mutex_t data_cond_mutex;
pthread_cond_t data_cond;

pthread_mutex_t db_dump_cond_mutex;
pthread_cond_t  db_dump_cond;

int shared_value = 0;

char shared_db_dump[256];
short shared_buf[9600];
short share_freq[2048];
char share_message_hw[256];
char share_message_sw[256];
char share_message_project[256];
int main(int argc, char *argv[]) {
    pthread_t rest_thr, mqtt_thr_sub, mqtt_thr_pub, print_thr, db_9_thr, alsa_thr, fft_thr;;
  
   
    
    // prepare IPC objects
    pthread_mutex_init(&data_cond_mutex, NULL);
    pthread_cond_init(&data_cond, NULL);
    pthread_mutex_init(&db_dump_cond_mutex, NULL);
    pthread_cond_init(&db_dump_cond, NULL);
    

    // initialize threads
    // pthread_create(&rest_thr, NULL, rest_thr_fcn, (void*)argv[1]);
    // pthread_create(&mqtt_thr_sub, NULL, mqtt_thr_fcn_sub, (void*)argv[3]);
    // pthread_create(&print_thr, NULL, message_print, (void*)argv[2]);
    // pthread_create(&db_9_thr, NULL, db_9_cmd_thr, (void*)argv[2]);
    pthread_create(&alsa_thr, NULL, alsa_thr_fcn, "plughw:3,0");
    pthread_create(&fft_thr, NULL, fft_thr_fcn, (void*)argv[1]);
    // pthread_create(&mqtt_thr_pub, NULL, mqtt_thr_fcn, (void*)argv[2]);

    // waiting for all thread terminates
    // pthread_join(rest_thr, NULL);
    // pthread_join(print_thr, NULL);
    // pthread_join(mqtt_thr_pub, NULL); 
    // pthread_join(mqtt_thr_sub, NULL); 
    // pthread_join(db_9_thr, NULL);
    pthread_join(alsa_thr, NULL);
    pthread_join(fft_thr, NULL);

}