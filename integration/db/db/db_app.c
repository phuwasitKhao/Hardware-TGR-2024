#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db_helper.h"

void usage()
{
    printf("Usage: db_app <DB_TYPE> <CMD> [ARG]\n");
    printf("DB_TYPE: LOGS | AUDIO | FEATURES\n");
    printf("CMD: INIT | ADD | QUERY\n");
    printf("For ADD, provide an additional ARG for value.\n");
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        usage();
        return 1;
    }

    const char *db_type = argv[1];
    const char *cmd = argv[2];
    const char *db_name = NULL;

    // Map DB_TYPE to corresponding database file
    if (strcmp(db_type, "LOGS") == 0)
    {
        db_name = "logs.db";
    }
    else if (strcmp(db_type, "AUDIO") == 0)
    {
        db_name = "audio.db";
    }
    else if (strcmp(db_type, "FEATURES") == 0)
    {
        db_name = "features.db";
    }
    else
    {
        printf("Unknown DB_TYPE %s\n", db_type);
        usage();
        return 1;
    }

    if (strcmp(cmd, "INIT") == 0)
    {
        // Initialize the database based on the type
        if (strcmp(db_type, "LOGS") == 0)
        {
            dbase_init(db_name, "CREATE TABLE IF NOT EXISTS logs ("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "timestamp DATETIME DEFAULT (DATETIME('now', 'localtime')), "
                                "event_type TEXT, "
                                "details TEXT)");
        }

        else if (strcmp(db_type, "AUDIO") == 0)
        {
            dbase_init(db_name, "CREATE TABLE IF NOT EXISTS audio_files ("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "file_name TEXT NOT NULL, "
                                "file_path TEXT NOT NULL, "
                                "file_size INTEGER, "
                                "created_at DATETIME DEFAULT (DATETIME('now', 'localtime')))");
        }
        else if (strcmp(db_type, "FEATURES") == 0)
        {
            dbase_init(db_name, "CREATE TABLE IF NOT EXISTS audio_features ("
                                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                "audio_id INTEGER NOT NULL, "
                                "feature_data TEXT NOT NULL, "
                                "processed_at DATETIME DEFAULT (DATETIME('now', 'localtime')))");
        }
        printf("%s initialized.\n", db_name);
    }
    else if (strcmp(cmd, "ADD") == 0)
    {
        if (argc < 4)
        {
            printf("ADD requires an additional argument.\n");
            usage();
            return 1;
        }
        else if (strcmp(db_type, "LOGS") == 0)
        {
            sqlite3 *db;
            sqlite3_stmt *stmt;

            if (sqlite3_open(db_name, &db) != SQLITE_OK)
            {
                fprintf(stderr, "Error opening database: %s\n", db_name);
                return 1;
            }

            const char *sql = "INSERT INTO logs (event_type, details) VALUES (?, ?)";
            if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
            {
                fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
                sqlite3_close(db);
                return 1;
            }

            // ใช้ค่า `EVENT_TYPE` สำหรับ `event_type` และ `argv[3]` สำหรับ `details`
            sqlite3_bind_text(stmt, 1, "EVENT_TYPE", -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, argv[3], -1, SQLITE_STATIC);

            if (sqlite3_step(stmt) != SQLITE_DONE)
            {
                fprintf(stderr, "Error executing statement: %s\n", sqlite3_errmsg(db));
            }
            else
            {
                printf("Added log event with details: %s to %s.\n", argv[3], db_name);
            }

            sqlite3_finalize(stmt);
            sqlite3_close(db);
        }

        else if (strcmp(db_type, "AUDIO") == 0)
        {
            sqlite3 *db;
            sqlite3_stmt *stmt;

            if (sqlite3_open(db_name, &db) != SQLITE_OK)
            {
                fprintf(stderr, "Error opening database: %s\n", db_name);
                return 1;
            }

            const char *sql = "INSERT INTO audio_files (file_name, file_path, file_size) VALUES (?, ?, ?)";
            if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
            {
                fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
                sqlite3_close(db);
                return 1;
            }

            sqlite3_bind_text(stmt, 1, "example_file", -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, "/path/to/example_file", -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 3, atoi(argv[3]));

            if (sqlite3_step(stmt) != SQLITE_DONE)
            {
                fprintf(stderr, "Error executing statement: %s\n", sqlite3_errmsg(db));
            }
            else
            {
                printf("Added audio metadata to %s.\n", db_name);
            }

            sqlite3_finalize(stmt);
            sqlite3_close(db);
        }
    }
    else if (strcmp(cmd, "QUERY") == 0)
    {
        sqlite3 *db;
        sqlite3_stmt *stmt;

        const char *query_sql = "SELECT id, event_type, DATETIME(timestamp, 'localtime') AS local_timestamp, details "
                                "FROM logs ORDER BY timestamp DESC LIMIT 1";

        if (sqlite3_open(db_name, &db) != SQLITE_OK)
        {
            fprintf(stderr, "Error opening database: %s\n", db_name);
            return 1;
        }

        if (sqlite3_prepare_v2(db, query_sql, -1, &stmt, NULL) != SQLITE_OK)
        {
            fprintf(stderr, "Error preparing SQL query: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return 1;
        }

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            printf("Query Result:\n");
            printf("ID: %d\n", sqlite3_column_int(stmt, 0));
            printf("Data: %s\n", sqlite3_column_text(stmt, 1));
            printf("Local Timestamp: %s\n", sqlite3_column_text(stmt, 2));
            printf("Details: %s\n", sqlite3_column_text(stmt, 3));
        }
        else
        {
            printf("No data found in %s.\n", db_name);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }

    else
    {
        printf("Unknown command %s\n", cmd);
        usage();
    }

    return 0;
}
