#include "db_helper.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// SQL commands
const char INIT_SQL_CMD[] = "CREATE TABLE IF NOT EXISTS logs ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, "
                            "outcome TEXT NOT NULL, "
                            "filename TEXT NOT NULL);";

const char APPEND_SQL_CMD[] = "INSERT INTO logs (outcome, filename) VALUES (?, ?);";

const char QUERY_SQL_CMD_ALL[] = "SELECT id, timestamp, outcome, filename FROM logs;";
const char QUERY_SQL_CMD_FILTER[] = "SELECT id, timestamp, outcome, filename FROM logs WHERE outcome = ?;";

// Initialize the database
void dbase_init(const char *db_name)
{
    sqlite3 *db;

    if (sqlite3_open(db_name, &db) != SQLITE_OK)
    {
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        return;
    }

    if (sqlite3_exec(db, INIT_SQL_CMD, NULL, NULL, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error executing SQL command: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    printf("Database initialized successfully.\n");
    sqlite3_close(db);
}

// Append a log to the database
int dbase_append(const char *db_name, const char *outcome, const char *filename)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (sqlite3_open(db_name, &db) != SQLITE_OK)
    {
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    if (sqlite3_prepare_v2(db, APPEND_SQL_CMD, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, outcome, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, filename, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "Error executing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    printf("Log added: [%s] %s\n", outcome, filename);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

// Query logs from the database
void dbase_query(const char *db_name, const char *filter)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (sqlite3_open(db_name, &db) != SQLITE_OK)
    {
        fprintf(stderr, "Error opening database: %s\n", sqlite3_errmsg(db));
        return;
    }

    const char *sql = filter ? QUERY_SQL_CMD_FILTER : QUERY_SQL_CMD_ALL;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    if (filter)
    {
        sqlite3_bind_text(stmt, 1, filter, -1, SQLITE_STATIC);
    }

    printf("Query Results:\n");
    printf("ID | Timestamp           | Outcome        | Filename\n");
    printf("-----------------------------------------------------\n");

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        const char *timestamp = (const char *)sqlite3_column_text(stmt, 1);
        const char *outcome = (const char *)sqlite3_column_text(stmt, 2);
        const char *filename = (const char *)sqlite3_column_text(stmt, 3);

        printf("%2d | %s | %-14s | %s\n", id, timestamp, outcome, filename);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
