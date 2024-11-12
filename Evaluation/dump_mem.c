#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_mem_free()
{
    FILE *fh;
    char buf[5000];
    char *tmp_buf;
    int mem_free_line = 0;
    int mem_free_size = 0;

    fh = fopen("/proc/meminfo", "r");
    fread(buf, 5000, sizeof(char), fh);
    fclose(fh);
    tmp_buf = strtok(buf, "\n ");
    while (tmp_buf)
    {
        if (!mem_free_line)
        {
            if (strcmp(tmp_buf, "MemFree:") == 0)
            {
                mem_free_line = 1;
            }
        }
        else
        {
            mem_free_size = atoi(tmp_buf);
            break;
        }
        tmp_buf = strtok(NULL, "\n ");
    }
    return mem_free_size;
}

int main()
{
    const char db_name[] = "mem.db";
    int mem_free_size = get_mem_free();
    printf("Meme => %d\n", mem_free_size);
    exit(0);
}