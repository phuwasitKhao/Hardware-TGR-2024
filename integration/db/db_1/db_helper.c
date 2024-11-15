#include "db_helper.h"

// Private constants
const char INIT_SQL_CMD[] = "CREATE TABLE IF NOT EXISTS data_table (\
    _id INTEGER PRIMARY KEY AUTOINCREMENT, \
    value FLOAT, \
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP \
);";

const char APPEND_SQL_CMD[] = "INSERT INTO data_table (value) VALUES (?)";

const char QUERY_SQL_CMD[] = "SELECT _id,value,timestamp FROM data_table ORDER BY timestamp DESC LIMIT 1";

// Initialize database
void dbase_init(const char *db_name)
{
    sqlite3 *db;

    if (sqlite3_open(db_name, &db) != SQLITE_OK)
    {
        fprintf(stderr, "Error opening database: %s\n", db_name);
        return;
    }

    if (sqlite3_exec(db, INIT_SQL_CMD, NULL, NULL, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error executing SQL: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_close(db);
}

// Append data to the database
int dbase_append(const char *db_name, double value)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;

    if (sqlite3_open(db_name, &db) != SQLITE_OK)
    {
        fprintf(stderr, "Error opening database: %s\n", db_name);
        return -1;
    }

    if (sqlite3_prepare_v2(db, APPEND_SQL_CMD, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_double(stmt, 1, value); // Bind double value to the SQL statement

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        fprintf(stderr, "Error executing SQL statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

// Query the last value from the database
double dbase_query(const char *db_name)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    double last_value = -1.0;

    if (sqlite3_open(db_name, &db) != SQLITE_OK)
    {
        fprintf(stderr, "Error opening database: %s\n", db_name);
        return -1.0;
    }

    if (sqlite3_prepare_v2(db, QUERY_SQL_CMD, -1, &stmt, NULL) != SQLITE_OK)
    {
        fprintf(stderr, "Error preparing SQL statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1.0;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);                          // ดึงค่า _id
        double value = sqlite3_column_double(stmt, 1);                 // ดึงค่า value
        const unsigned char *timestamp = sqlite3_column_text(stmt, 2); // ดึงค่า timestamp

        printf("Query Results:\n");
        printf("ID | Timestamp           | Timestamp        | Filename\n");
        // printf("-----------------------------------------------------\n");

        printf("%d | %.2f              | %s        ", id, value, timestamp); // แสดงผลตามลำดับที่ต้องการ
        last_value = value;                                                   // คืนค่า value ถ้าต้องการใช้ต่อ
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return last_value;
}
