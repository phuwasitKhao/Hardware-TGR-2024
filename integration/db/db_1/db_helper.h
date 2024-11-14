#ifndef DB_HELPER_H
#define DB_HELPER_H

#include <sqlite3.h>
#include <stdio.h>

// Function prototypes
void dbase_init(const char *db_name);
int dbase_append(const char *db_name, double value); // Update to match the double type
double dbase_query(const char *db_name);             // Update to return double type

#endif // DB_HELPER_H
