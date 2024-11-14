#include "main.h"
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

#define FIREBASE_URL "https://hardware-day1-tgr.firebaseio.com/sensorData.json"

#define FIREBASE_API_KEY "AIzaSyBE6E4pQPANKIe3dsoZrnGU25RUwYEj6WE"


void send_to_firebase(const char *data)
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl)
    {
        char url[512];
        snprintf(url, sizeof(url), "%s?auth=%s", FIREBASE_URL, FIREBASE_API_KEY);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, resp_callback);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
}

size_t resp_callback(void *buffer, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    printf("Firebase Response: %s\n", (char *)buffer);
    return realsize;
}

void *rest_thr_fcn(void *ptr)
{
    printf("Starting Firebase REST API thread.\n");

    while (1)
    {
        pthread_mutex_lock(&data_cond_mutex);
        pthread_cond_wait(&data_cond, &data_cond_mutex);

        char mes[256];
        int mem = shared_value;

        strncpy(mes, shared_message, sizeof(mes) - 1);
        mes[sizeof(mes) - 1] = '\0';

        cJSON *json = cJSON_CreateObject();
        if (!json)
        {
            fprintf(stderr, "Failed to create JSON object\n");
            pthread_mutex_unlock(&data_cond_mutex);
            continue;
        }

        cJSON_AddStringToObject(json, "mem_type", mes);
        cJSON_AddNumberToObject(json, "mem_size", mem);

        char *json_string = cJSON_PrintUnformatted(json);
        if (json_string == NULL)
        {
            fprintf(stderr, "Failed to print JSON object\n");
            cJSON_Delete(json);
            pthread_mutex_unlock(&data_cond_mutex);
            continue;
        }

        send_to_firebase(json_string);

        cJSON_Delete(json);
        free(json_string);

        pthread_mutex_unlock(&data_cond_mutex);
    }

    return NULL;
}
