#include "main_thr.h"
#include "db/db_9/db_helper.h"
#include "model/predictML.h"
#include "model/predictML_terminate.h"
#include "db/db_7/db_thr.c"
#include <alsa/asoundlib.h>

pthread_mutex_t data_cond_mutex;
pthread_cond_t data_cond;

pthread_mutex_t db_dump_cond_mutex;
pthread_cond_t db_dump_cond;

pthread_mutex_t audio_cond_mutex;
pthread_cond_t audio_cond;

pthread_mutex_t rest_cond_mutex;
pthread_cond_t rest_cond;

pthread_mutex_t predict_mutex;
pthread_cond_t predict_cond;

int shared_value = 0;

char shared_db_dump[256];
short shared_buf[9600];
short share_freq[2048];
char shared_message[256];
char shared_message_sw[256];
char shared_name_ouput_file[256];
// int shared_predict = 0;


int main(int argc, char *argv[])
{
    pthread_t rest_thr, mqtt_thr_sub, mqtt_thr_pub, print_thr, db_9_thr, db_1_thr, alsa_thr, fft_thr, predict_thr, mqtt_fre_data;

    // Initialize mutexes and conditions
    pthread_mutex_init(&data_cond_mutex, NULL);
    pthread_cond_init(&data_cond, NULL);
    pthread_mutex_init(&db_dump_cond_mutex, NULL);
    pthread_cond_init(&db_dump_cond, NULL);
    pthread_mutex_init(&audio_cond_mutex, NULL);
    pthread_cond_init(&audio_cond, NULL);

    // Initialize threads
    pthread_create(&alsa_thr, NULL, alsa_thr_fcn, "plughw:2,0");
    pthread_create(&fft_thr, NULL, fft_thr_fcn, (void *)argv[1]);
    pthread_create(&rest_thr, NULL, rest_thr_fcn, (void *)argv[2]);
    pthread_create(&predict_thr, NULL, predict_thread, NULL);
    pthread_create(&mqtt_thr_sub, NULL, mqtt_thr_fcn_sub, (void *)argv[2]);
    // pthread_create(&print_thr, NULL, message_print, (void *)argv[2]);
    pthread_create(&db_9_thr, NULL, db_9_cmd_thr, (void *)argv[2]);
    pthread_create(&db_1_thr, NULL, db_1_cmd_thr, (void *)argv[2]);
    pthread_create(&mqtt_thr_pub, NULL, mqtt_thr_fcn, (void *)argv[2]);
    pthread_create(&mqtt_fre_data, NULL, mqtt_thr_fcn_freq, "test");

    // Wait for all threads to finish
    pthread_join(alsa_thr, NULL);
    pthread_join(fft_thr, NULL);
    pthread_join(rest_thr, NULL);
    // pthread_join(print_thr, NULL);
    pthread_join(mqtt_thr_pub, NULL);
    pthread_join(mqtt_thr_sub, NULL);
    pthread_join(db_9_thr, NULL);
    pthread_join(db_1_thr, NULL);
    pthread_join(mqtt_fre_data, NULL);
    pthread_join(predict_thr, NULL);

    // Terminate the prediction model
    predictML_terminate();
    return 0;
}
