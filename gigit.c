#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ftw.h>
#include <windows.h>

#define test printf("test\n");

int config(int argc, char *argv[]);
int init();
char *gigitExists(char *path);
int alias(int argc, char *argv[]);
int aliasCheck(int argc, char *argv[]);
int add(int argc, char *argv[]);
int mkdirRecursive(char *path);
int letterCount(char *str, char token);
int deleteStar(char *path);

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
    else if (strcmp(argv[1], "config") == 0)
    {
        printf("config\n");
        if (config(argc, argv))
            return 1;
    }
    else if (strcmp(argv[1], "add") == 0)
    {
        if (add(argc, argv))
            return 1;
    }
    else
    {
        if (aliasCheck(argc, argv))
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
    if (gigitExists(path) != NULL)
    {
        printf("Already a gigit repository\n");
        closedir(dir);
        return 1;
    }
    char *currentPath = "C:\\Users\\Admin\\OneDrive\\Desktop\\SUT\\gigit\\gigit\\config\\currentProjectPath.txt";
    char *linkPath = "C:\\Users\\Admin\\OneDrive\\Desktop\\SUT\\gigit\\gigit\\config\\projectsPath.txt";
    char *usernamepath = "C:\\Users\\Admin\\OneDrive\\Desktop\\SUT\\gigit\\gigit\\config\\username.txt";
    char *useremailpath = "C:\\Users\\Admin\\OneDrive\\Desktop\\SUT\\gigit\\gigit\\config\\useremail.txt";
    if (fopen(usernamepath, "r") == NULL)
    {
        printf("enter username and email using gigit config before init\n");
        return 1;
    }
    if (fopen(useremailpath, "r") == NULL)
    {
        printf("enter username and email using gigit config before init\n");
        return 1;
    }
    mkdir(".gigit");
    strcat(path, "\\.gigit");
    FILE *projectLink = fopen(linkPath, "a");
    fprintf(projectLink, "%s\n", path);
    fclose(projectLink);
    FILE *currentProjectLink = fopen(currentPath, "w");
    fprintf(currentProjectLink, "%s\n", path);
    fclose(currentProjectLink);
    char *configPath = ".gigit\\config";
    mkdir(configPath);
    char *usernamePath = ".gigit\\config\\username.txt";
    FILE *usernameFile = fopen(usernamePath, "w");
    char username[100];
    FILE *usernameFile2 = fopen(usernamepath, "r");
    fgets(username, 100, usernameFile2);
    username[strlen(username)] = '\0';
    fprintf(usernameFile, "%s\n", username);
    fclose(usernameFile);
    char *useremailPath = ".gigit\\config\\useremail.txt";
    FILE *useremailFile = fopen(useremailPath, "w");
    char useremail[100];
    FILE *useremailFile2 = fopen(useremailpath, "r");
    fgets(useremail, 100, useremailFile2);
    useremail[strlen(useremail)] = '\0';
    fprintf(useremailFile, "%s\n", useremail);
    fclose(useremailFile);
    closedir(dir);
    printf("Initialized empty gigit repository in %s\n", path);
    return 0;
}

char *gigitExists(char *path)
{
    deleteStar(path);
    DIR *dir = opendir(path);
    if (!dir)
    {
        return NULL;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".gigit") == 0)
        {
            closedir(dir);
            return strcat(path, "\\.gigit");
        }
    }

    closedir(dir);

    if (strcmp(path, "C:") == 0)
    {
        return NULL;
    }

    char *str = strrchr(path, '\\');
    if (str != NULL)
    {
        str[0] = '\0';
        return gigitExists(path);
    }

    return NULL;
}

int config(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Invalid command\n");
        return 1;
    }
    int user = 2;
    bool global = false;

    if (strcmp(argv[2], "-global") == 0)
    {
        global = true;
        user = 3;
    }
    if (strncmp(argv[user], "alias.", 6) == 0)
    {
        alias(argc, argv);
        return 0;
    }
    if (strcmp(argv[user], "user.name") == 0 && argc == user + 2)
    {
        FILE *file = fopen("gigit\\config\\username.txt", "w");

        if (!file)
        {
            printf("Error opening gigit\\config file");
            return 1;
        }

        fprintf(file, "%s", argv[user + 1]);
        fclose(file);
    }
    else if (strcmp(argv[user], "user.email") == 0 && argc == user + 2)
    {
        FILE *file = fopen("gigit\\config\\useremail.txt", "w");

        if (!file)
        {
            printf("Error opening gigit\\config file");
            return 1;
        }

        fprintf(file, "%s", argv[user + 1]);
        fclose(file);
    }
    else
    {
        printf("Invalid command\n");
        return 1;
    }
    if (global)
    {
        char *linkPath = "C:\\Users\\Admin\\OneDrive\\Desktop\\SUT\\gigit\\gigit\\config\\projectsPath.txt";
        FILE *projectsLink = fopen(linkPath, "r");
        char *path = NULL;
        char *read;
        while ((read = fgets(path, 200, projectsLink)) != NULL)
        {
            path[strlen(path) - 1] = '\0';
            if (!strcmp(argv[user], "user.name"))
            {
                char *usernamePath = strcat(path, "\\.gigit\\config\\username.txt");
                FILE *usernameFile = fopen(usernamePath, "w");
                fprintf(usernameFile, "%s\n", argv[user + 1]);
                fclose(usernameFile);
            }
            else if (!strcmp(argv[user], "user.email"))
            {
                char *useremailPath = strcat(path, "\\.gigit\\config\\useremail.txt");
                FILE *useremailFile = fopen(useremailPath, "w");
                fprintf(useremailFile, "%s\n", argv[user + 1]);
                fclose(useremailFile);
            }
        }
    }
    else
    {
        char *currentPath = "C:\\Users\\Admin\\OneDrive\\Desktop\\SUT\\gigit\\gigit\\config\\currentProjectPath.txt";
        FILE *currentProjectLink = fopen(currentPath, "r");
        if (!currentProjectLink)
        {
            printf("Error opening currentProjectPath.txt\n");
            return 1;
        }
        char *path = NULL;
        char *read;
        while ((read = fgets(path, 200, currentProjectLink)) != NULL)
        {
            path[strlen(path) - 1] = '\0';
            if (!strcmp(argv[user], "user.name"))
            {
                char *usernamePath = strcat(path, "\\.gigit\\config\\username.txt");
                FILE *usernameFile = fopen(usernamePath, "w");
                fprintf(usernameFile, "%s\n", argv[user + 1]);
                fclose(usernameFile);
            }
            else if (!strcmp(argv[user], "user.email"))
            {
                char *useremailPath = strcat(path, "\\.gigit\\config\\useremail.txt");
                FILE *useremailFile = fopen(useremailPath, "w");
                fprintf(useremailFile, "%s\n", argv[user + 1]);
                fclose(useremailFile);
            }
        }
    }
    printf("configed succesfuly\n");
}

int alias(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Invalid command\n");
        return 1;
    }
    int user = 2;
    bool global = false;

    if (strcmp(argv[2], "-global") == 0)
    {
        global = true;
        user = 3;
    }
    if (strncmp(argv[user], "alias.", 6) == 0 && argc == user + 2)
    {
        char *aliasPath;
        if (!global)
        {
            DIR *dir = opendir(".");
            if (!dir)
            {
                printf("Error: not a directory\n");
                return 1;
            }
            char *path = dir->dd_name;
            if (gigitExists(path) == NULL)
            {
                printf("Not a gigit repository\n");
                closedir(dir);
                return 1;
            }
            aliasPath = strcat(path, "\\config\\alias.txt");
        }
        else
        {
            aliasPath = "C:\\Users\\Admin\\OneDrive\\Desktop\\SUT\\gigit\\gigit\\config\\alias.txt";
        }
        FILE *aliasLink = fopen(aliasPath, "a");
        char *aliasName = argv[user] + 6;
        fprintf(aliasLink, "%s %s\n", aliasName, argv[user + 1]);
        printf("alias added succesfuly\n");
    }
}

int aliasCheck(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Invalid command\n");
        return 1;
    }
    bool aliasFound = false;
    char *aliasPath = "C:\\Users\\Admin\\OneDrive\\Desktop\\SUT\\gigit\\gigit\\config\\alias.txt";
    FILE *aliasLink = fopen(aliasPath, "r");
    char path[200];
    char *read;
    while ((read = fgets(path, 200, aliasLink)) != NULL)
    {
        path[strlen(path) - 1] = '\0';
        if (!strncmp(argv[1], path, strlen(argv[1])) && path[strlen(argv[1])] == ' ')
        {
            aliasFound = true;
            char *command = path + strlen(argv[1]) + 1;
            system(command);
            return 0;
        }
    }
    printf("invalid input or alias not found\n");
    return 1;
}

int add(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Invalid command\n");
        return 1;
    }
    if (argc == 3)
    {
        struct stat s;
        if (stat(argv[2], &s) == 0)
        {
            if (S_ISDIR(s.st_mode))
            {
                DIR *dir = opendir(argv[2]);
                if (!dir)
                {
                    printf("Error: not a directory\n");
                    return 1;
                }
                test char *path = malloc(sizeof(MAX_PATH));
                path = dir->dd_name;
                char *path2 = malloc(sizeof(path) + sizeof("\\.gigit"));
                strcpy(path2, path);
                char *gigitpath = malloc(sizeof(MAX_PATH));
                gigitpath = gigitExists(path2);
                if (gigitpath == NULL)
                {
                    printf("Not a gigit repository\n");
                    closedir(dir);
                    return 1;
                }
                char *relativePath = path + strlen(gigitpath) - strlen("\\.gigit");
                char *savePath = malloc(strlen(gigitpath) + strlen("\\stage") + strlen(relativePath) + 1);
                if (savePath == NULL)
                {
                    perror("malloc failed");
                    return 1;
                }
                strcpy(savePath, gigitpath);
                strcat(savePath, "\\stage");
                strcat(savePath, relativePath);
                printf("%s\n", savePath);
                mkdirRecursive(savePath);
                struct dirent *entry;
                while ((entry = readdir(dir)) != NULL)
                {
                    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
                    {
                        printf("%s\n", entry->d_name);
                        deleteStar(path);
                        char *newPath = strcat(path, "\\");
                        newPath = strcat(newPath, entry->d_name);
                        printf("%s\n", newPath);
                        add(3, (char *[]){"gigit", "add", newPath});
                    }
                }
            }
            else
            {
                // argv[2] is not a directory
                // Buffer to store the full path
                char fullPath[MAX_PATH];

                // Get the full path
                DWORD result = GetFullPathNameA(argv[2], MAX_PATH, fullPath, NULL);

                if (result == 0)
                {
                    // Error handling
                    perror("GetFullPathName failed");
                    return 1;
                }
                char *path = malloc(strlen(fullPath));
                strcpy(path, fullPath);
                strrchr(path, '\\')[0] = '\0';
                char *gigitpath = gigitExists(path);
                if (gigitpath == NULL)
                {
                    printf("Not a gigit repository\n");
                    return 1;
                }
                test char *relativePath = path + (strlen(gigitpath) - strlen("\\.gigit"));
                char *savePath = malloc(strlen(gigitpath) + strlen("\\stage") + strlen(relativePath) + 1);
                if (savePath == NULL)
                {
                    perror("malloc failed");
                    return 1;
                }
                strcpy(savePath, gigitpath);
                strcat(savePath, "\\stage");
                strcat(savePath, relativePath);
                char *removeFileFromPath = strrchr(savePath, '\\');
                removeFileFromPath[0] = '\0';
                char *lastPath = malloc(strlen(fullPath) + strlen("\\stage") + strlen(relativePath) + 1);
                if (lastPath == NULL)
                {
                    perror("malloc failed");
                    return 1;
                }
                char *temp = malloc(strlen(fullPath) + 1);
                if (temp == NULL)
                {
                    perror("malloc failed");
                    return 1;
                }
                strcpy(temp, fullPath);
                temp = temp + strlen(gigitpath) - strlen("\\.gigit");
                strcpy(lastPath, savePath);
                mkdirRecursive(lastPath);
                strcat(lastPath, temp);
                if (access(lastPath, F_OK) == 0)
                {
                    struct stat f;
                    if (stat(lastPath, &f) == 0)
                    {
                        if (!S_ISDIR(f.st_mode))
                        {
                            printf("file already added\n");
                            return 1;
                        }
                    }
                }
                FILE *file = fopen(lastPath, "w");
                FILE *file2 = fopen(argv[2], "r");
                char line[BUFSIZ];
                while (fgets(line, sizeof(line), file2) != NULL)
                {
                    fputs(line, file);
                }
                fclose(file);
                fclose(file2);
            }
        }
        else
        {
            // error
            printf("directory or file doesn't exists\n");
            return 1;
        }
        printf("added succesfuly\n");
        return 0;
    }
}

int letterCount(char *str, char token)
{
    int count = 0;
    while (str[0] != '\0')
    {
        if (str[0] == token)
            count++;
        str++;
    }
    return count;
}

int mkdirRecursive(char *path)
{
    if (!strcmp(path, "C:"))
    {
        return 0;
    }
    char *path2 = malloc(strlen(path));
    strcpy(path2, path);
    char *token = strrchr(path2, '\\');
    token[0] = '\0';
    mkdirRecursive(path2);
    mkdir(path);
    free(path2);
    return 0;
}

int deleteStar(char *path)
{
    if (path[strlen(path) - 2] == '\\' && path[strlen(path) - 1] == '*')
    {
        path[strlen(path) - 2] = '\0';
    }
    return 0;
}