// #include "thr_app.h"
// #include <string.h>
// #include <cjson/cJSON.h>
// // private constants

// // private variables

// // private functions

// int is_valid_memory_type(const char *memory_type)
// {
//     const char *valid_types[] = {"MemTotal", "MemFree", "MemAvailable"};
//     int num_types = sizeof(valid_types) / sizeof(valid_types[0]);

//     for (int i = 0; i < num_types; i++)
//     {
//         if (strcmp(memory_type, valid_types[i]) == 0)
//         {
//             return 1;
//         }
//     }
//     return 0;
// }

// void *end_thr_fcn(void *ptr)
// {

//     int value = 0;
//     time_t now;
//     struct tm *timeinfo;

//     const char *json_data = "{\"check\": \"MemFree\"}";
//     while (1)
//     {
//         cJSON *json = cJSON_Parse(json_data);
//         if (json == NULL)
//         {
//             fprintf(stderr, "Error parsing JSON\n");
//             continue;
//         }

//         cJSON *check = cJSON_GetObjectItem(json, "check");
//         if (cJSON_IsString(check) && (check->valuestring != NULL))
//         {
//             if (is_valid_memory_type(check->valuestring))
//             {
//                 strcpy(memory_type, check->valuestring);
//                 shared_data = 12345; // ตัวอย่างค่า memory ที่รับมา
//                 printf("[End] Valid memory type: %s, Value: %d\n", memory_type, shared_data);

//                 pthread_mutex_lock(&data_cond_mutex);
//                 pthread_cond_signal(&data_cond);
//                 pthread_mutex_unlock(&data_cond_mutex);
//             }
//             else
//             {
//                 printf("[End] Invalid memory type: %s. Ignored.\n", check->valuestring);
//             }
//         }

//         cJSON_Delete(json);

//     }
//     return NULL;
// }