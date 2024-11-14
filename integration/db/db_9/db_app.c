#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db_helper.h"

void usage() {
    printf("Usage: db_app <DB_TYPE> <CMD> [ARG]\n");
    printf("DB_TYPE: LOGS\n");
    printf("CMD: INIT | ADD | QUERY\n");
    printf("For ADD, provide an additional ARG for event details.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        usage();
        return 1;
    }

    const char *db_type = argv[1];
    const char *cmd = argv[2];
    const char *db_name = "logs.db";

    if (strcmp(db_type, "LOGS") != 0) {
        printf("Unsupported DB_TYPE: %s\n", db_type);
        usage();
        return 1;
    }

    if (strcmp(cmd, "INIT") == 0) {
        if (dbase_init(db_name, "CREATE TABLE IF NOT EXISTS logs ("
                                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                 "timestamp DATETIME DEFAULT (DATETIME('now', 'localtime')), "
                                 "details TEXT)") == 0) {
            printf("%s initialized.\n", db_name);
        } else {
            printf("Failed to initialize database: %s\n", db_name);
        }
    } else if (strcmp(cmd, "ADD") == 0) {
        if (argc < 4) {
            printf("ADD requires an additional argument for details.\n");
            usage();
            return 1;
        }
        const char *details = argv[3];
        if (dbase_append(db_name, "logs", details) == 0) {
            printf("Added log event: %s\n", details);
        } else {
            printf("Failed to add log event.\n");
        }
    } else if (strcmp(cmd, "QUERY") == 0) {
        if (dbase_query(db_name, "SELECT id, details, timestamp FROM logs ORDER BY timestamp DESC LIMIT 1") != 0) {
            printf("Failed to query database.\n");
        }
    } else {
        printf("Unknown command %s\n", cmd);
        usage();
    }

    return 0;
}
