#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_memory_info(int *mem_total, int *mem_free, int *mem_available) {
    FILE *fh;
    char buf[256];

    *mem_total = 0;
    *mem_free = 0;
    *mem_available = 0;

    fh = fopen("/proc/meminfo", "r");
    if (fh == NULL) {
        perror("Failed to open /proc/meminfo");
        return;
    }

    while (fgets(buf, sizeof(buf), fh)) {
        if (strncmp(buf, "MemTotal:", 9) == 0) {
            sscanf(buf, "MemTotal: %d kB", mem_total);
        } else if (strncmp(buf, "MemFree:", 8) == 0) {
            sscanf(buf, "MemFree: %d kB", mem_free);
        } else if (strncmp(buf, "MemAvailable:", 13) == 0) {
            sscanf(buf, "MemAvailable: %d kB", mem_available);
        }
    }

    fclose(fh);
}

int main() {
    int mem_total, mem_free, mem_available;
    get_memory_info(&mem_total, &mem_free, &mem_available);

    printf("MemTotal: %d kB\n", mem_total);
    printf("MemFree: %d kB\n", mem_free);
    printf("MemAvailable: %d kB\n", mem_available);

    return 0;
}
