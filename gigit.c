#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>

#define INVALID printf("Invalid command\n");

int init();

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        INVALID
        return 1;
    }
    if (strcmp(argv[1], "init") == 0)
    {
        if (init())
            return 1;
        return 0;
    }
}

int init()
{
    DIR *dir = opendir(".gigit");
    char *path = dir->dd_name;
    if (gigitExists(path))
        return 1;
    mkdir(".gigit", 0777);
}

int gigitExists(char *path)
{
    DIR *dir = opendir(path);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".gigit") == 0)
        {
            return 1;
        }
    }
    if (!strcmp(path, "C:"))
    {
        return 0;
    }
    char *str = strrchr(path, '/');
    str[0] = '\0';
    gigitExists(path);
}