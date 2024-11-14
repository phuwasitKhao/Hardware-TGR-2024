#ifndef DB_HELPER_H
#define DB_HELPER_H

// Function prototypes
void dbase_init(const char *db_name, const char *sql_cmd);
int dbase_append(const char *db_name, const char *table_name, const char *details);
void dbase_query(const char *db_name, const char *query_sql);

#endif // DB_HELPER_H
