#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>

#define test printf("test\n");

int config(int argc, char *argv[]);
int init();
int gigitExists(char *path);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Invalid command\n");
        return 1;
    }
    if (strcmp(argv[1], "init") == 0)
    {
        if (init())
            return 1;
    }
    if (strcmp(argv[1], "config") == 0)
    {
        printf("config\n");
        if (config(argc, argv))
            return 1;
    }
    printf("hello world\n");
    return 0;
}

int init()
{
    DIR *dir;
    if ((dir = opendir(".")) == NULL)
    {
        printf("Error: not a directory\n");
        return 1;
    }
    char *path = dir->dd_name;
    if (gigitExists(path))
    {
        printf("Already a gigit repository\n");
        closedir(dir);
        return 1;
    }
    mkdir(".gigit");
    char *linkPath = "C:\\Users\\Admin\\OneDrive\\Desktop\\SUT\\gigit\\gigit\\config\\projects path.txt";
    FILE *projectLink = fopen(linkPath, "a");
    fprintf(projectLink, "%s\n", path);
    closedir(dir);
    return 0;
}

int gigitExists(char *path)
{
    DIR *dir = opendir(path);
    if (!dir)
    {
        return 0;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".gigit") == 0)
        {
            closedir(dir);
            return 1;
        }
    }

    closedir(dir);

    if (strcmp(path, ".") == 0)
    {
        return 0;
    }

    char *str = strrchr(path, '\\');
    if (str != NULL)
    {
        str[0] = '\0';
        return gigitExists(path);
    }

    return 0;
}

int config(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Invalid command\n");
        return 1;
    }

    bool global = false;

    if (strcmp(argv[2], "-global") == 0)
    {
        global = true;
    }
    if (strcmp(argv[3], "user.name") == 0 && argc == 5)
    {
        FILE *file = fopen("gigit\\config\\username.txt", "w");

        if (!file)
        {
            perror("Error opening .gigit\\config file");
            return 1;
        }

        fprintf(file, "%s", argv[4]);
        fclose(file);
    }
    else if (strcmp(argv[3], "user.email") == 0 && argc == 5)
    {
        FILE *file = fopen("gigit\\config\\useremail.txt", "w");

        if (!file)
        {
            perror("Error opening .gigit\\config file");
            return 1;
        }

        fprintf(file, "%s", argv[4]);
        fclose(file);
    }
    else
    {
        printf("Invalid command\n");
        return 1;
    }
    if (global)
    {
        // complete here
    }
}
