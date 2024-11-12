#include "db_helper.h"

// private constants
// private constants

// Command to create the table with necessary columns
const char INIT_SQL_CMD[] = "CREATE TABLE IF NOT EXISTS data_cmd (\
    _id INTEGER PRIMARY KEY AUTOINCREMENT, \
    timestamp DATETIME, \
    cmd VARCHAR(255), \
    mem_value INTEGER \
)";

// Command to insert a new record into the table
const char APPEND_SQL_CMD[] = "INSERT INTO data_cmd (cmd, mem_value, timestamp) VALUES (?, ?, ?)";

// Command to select the latest record (most recent timestamp)
const char QUERY_SQL_CMD[] = "SELECT * FROM data_cmd ORDER BY timestamp DESC LIMIT 1";

// private functions

// private variables

// initialize database
void dbase_init(const char *db_name)
{
    sqlite3 *db;

    if (sqlite3_open(db_name, &db) != SQLITE_OK)
    {
        fprintf(stderr, "Error opening %s database\n", db_name);
        sqlite3_close(db);
        return;
    }
    if (sqlite3_exec(db, INIT_SQL_CMD, NULL, NULL, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error executing %s: %s\n", INIT_SQL_CMD, sqlite3_errmsg(db));
        return;
    }
    sqlite3_close(db);
}

// Function to append data to the table
int dbase_append(const char *db_name, const char *cmd, int value, char *timestamp)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;

    // Open the database
    if (sqlite3_open(db_name, &db) != SQLITE_OK)
    {
        fprintf(stderr, "Error opening %s database\n", db_name);
        sqlite3_close(db);
        return -1;
    }

    // Prepare the SQL statement for execution
    if (sqlite3_prepare_v2(db, APPEND_SQL_CMD, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // Bind the 'cmd' parameter (the command string)
    sqlite3_bind_text(stmt, 1, cmd, -1, SQLITE_STATIC); // Use SQLITE_STATIC as the binding option for a string

    // Bind the 'mem_value' parameter (the integer value)
    sqlite3_bind_int(stmt, 2, value); // The second parameter binds the value

    sqlite3_bind_text(stmt, 3, timestamp, -1, SQLITE_STATIC); // Binding the timestamp

    // Execute the statement
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "Error executing statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0; // Successfully inserted data
}

// query last value from the table
int dbase_query(const char *db_name)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int last_value = -1;

    if (sqlite3_open(db_name, &db) != SQLITE_OK)
    {
        fprintf(stderr, "Error opening %s database\n", db_name);
        sqlite3_close(db);
        return -1;
    }
    if (sqlite3_prepare_v2(db, QUERY_SQL_CMD, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error executing %s: %s\n", QUERY_SQL_CMD, sqlite3_errmsg(db));
        return -1;
    }
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *timestamp = sqlite3_column_text(stmt, 1);
        printf("Data timestamp: %s\n", timestamp);
        last_value = sqlite3_column_int(stmt, 2);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return last_value;
}