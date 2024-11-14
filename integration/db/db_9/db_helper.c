#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db_helper.h"

// Initialize the database with a specific SQL command
int dbase_init(const char *db_name, const char *sql_cmd)
{
    sqlite3 *db;

    if (sqlite3_open(db_name, &db) != SQLITE_OK)
    {
        fprintf(stderr, "Error opening database: %s\n", db_name);
        sqlite3_close(db);
        return;
    }
    printf("startdeb");
    // Execute the provided SQL command
    if (sqlite3_exec(db, sql_cmd, NULL, NULL, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error executing SQL command: %s\n", sqlite3_errmsg(db));
    }
    else
    {
        printf("Database %s initialized successfully.\n", db_name);
    }

    sqlite3_close(db);
}

// Append a value to a specified table
int dbase_append(const char *db_name, const char *table_name, const char *details)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char sql[256];

    snprintf(sql, sizeof(sql), "INSERT INTO %s (details) VALUES (?)", table_name);

    if (sqlite3_open(db_name, &db) != SQLITE_OK)
    {
        fprintf(stderr, "Error opening database: %s\n", db_name);
        return -1;
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, details, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "Error executing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

// Query data from a database
int dbase_query(const char *db_name, const char *query_sql)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (sqlite3_open(db_name, &db) != SQLITE_OK)
    {
        fprintf(stderr, "Error opening database: %s\n", db_name);
        return;
    }

    if (sqlite3_prepare_v2(db, query_sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        printf("Query Result:\n");
        printf("ID: %d\n", sqlite3_column_int(stmt, 0));
        printf("Details: %s\n", sqlite3_column_text(stmt, 1));
        printf("Timestamp: %s\n", sqlite3_column_text(stmt, 2));
    }
    else
    {
        printf("No data found in the database.\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}
