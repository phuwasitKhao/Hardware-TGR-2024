#ifndef DB_HELPER_H
#define DB_HELPER_H

// include files
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

// function prototypes
void dbase_init_9(const char *db_name);
int dbase_append_9(const char *db_name, int value);
int dbase_query_9(const char *db_name);

#endif // DB_HELPER_H