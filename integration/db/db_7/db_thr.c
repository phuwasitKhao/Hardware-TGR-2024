#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// Shared data and mutexes
extern short shared_buf[9600];
extern pthread_mutex_t audio_cond_mutex;
extern pthread_cond_t audio_cond;
char shared_predict[20];

// Database file path
const char *DB_PATH = "logs_7.db";

// Helper function to initialize the database
void init_db(sqlite3 **db) {
    int rc = sqlite3_open(DB_PATH, db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*db));
        exit(1);
    }

    const char *sql_create_table = "CREATE TABLE IF NOT EXISTS Predictions ("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                   "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
                                   "position INTEGER, "
                                   "prediction INTEGER);";

    char *err_msg = NULL;
    rc = sqlite3_exec(*db, sql_create_table, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(*db);
        exit(1);
    }
}

// Function to save prediction results to the database
void save_prediction(sqlite3 *db, int position, int prediction) {
    const char *sql_insert = "INSERT INTO Predictions (position, prediction) VALUES (?, ?);";
    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, position);
    sqlite3_bind_int(stmt, 2, prediction);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        fprintf(stderr, "Failed to insert data: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

// Prediction thread function
void *predict_thread(void *ptr) {
    sqlite3 *db;
    init_db(&db);  // Initialize the database connection

    while (1) {
        pthread_mutex_lock(&audio_cond_mutex);
        pthread_cond_wait(&audio_cond, &audio_cond_mutex);

        // Convert audio data from short to double for predictML
        double audioData[48000];
        for (int i = 0; i < 48000 && i < 9600; ++i) {
            audioData[i] = (double)shared_buf[i] / 0x7FFF;
        }
        pthread_mutex_unlock(&audio_cond_mutex);

        // Prepare output arrays
        int positionArray_data[100];
        int positionArray_size[2];
        unsigned char predictNumArray_data[100];
        int predictNumArray_size[2];

        // Run prediction
        predictML(audioData, positionArray_data, positionArray_size, predictNumArray_data, predictNumArray_size);

        // Process and save prediction results
        if (positionArray_size[1] > 0 && predictNumArray_size[1] > 0) {
            int position = positionArray_data[0];
            int prediction = predictNumArray_data[0];
            
            // Save the prediction to the SQLite database
            save_prediction(db, position, prediction);

            // Display prediction results
            if (position == 70) {
                printf("F detected\n");
            }
            if (prediction == 78) {
                printf("N detected\n");
            }
        }
    }

    sqlite3_close(db);  // Close the database connection
    return NULL;
}
