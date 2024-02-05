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
int reset(int argc, char *argv[]);
int commit(int argc, char *argv[]);
int depth(int argc, char *argv[], int count);
int branch(int argc, char *argv[]);
int checkout(int argc, char *argv[]);
int log1(int argc, char *argv[]);
int logID(int ID);
int tag(int argc, char *argv[]);
int status(int argc, char *argv[]);
void printstatus(char *path, char *stagedPath);
int grep(int argc, char *argv[]);
char *fileFinding(char *path, char *filename);
int preCommit(int argc, char *argv[]);
int diff(int argc, char *argv[]);

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
        if (config(argc, argv))
            return 1;
    }
    else if (strcmp(argv[1], "add") == 0)
    {
        if (add(argc, argv))
            return 1;
        else
            printf("added succesfuly\n");
    }
    else if (strcmp(argv[1], "reset") == 0)
    {
        if (reset(argc, argv))
            return 1;
        printf("reset succesfuly\n");
    }
    else if (strcmp(argv[1], "commit") == 0)
    {
        if (commit(argc, argv))
            return 1;
    }
    else if (strcmp(argv[1], "branch") == 0)
    {
        if (branch(argc, argv))
            return 1;
    }
    else if (strcmp(argv[1], "checkout") == 0)
    {
        if (checkout(argc, argv))
            return 1;
    }
    else if (strcmp(argv[1], "log") == 0)
    {
        if (log1(argc, argv))
            return 1;
    }
    else if (strcmp(argv[1], "tag") == 0)
    {
        if (tag(argc, argv))
            return 1;
    }
    else if (strcmp(argv[1], "status") == 0)
    {
        if (status(argc, argv))
            return 1;
    }
    else if (strcmp(argv[1], "grep") == 0)
    {
        if (grep(argc, argv))
            return 1;
    }
    else if (strcmp(argv[1], "pre-commit") == 0)
    {
        if (preCommit(argc, argv))
            return 1;
    }
    else if (strcmp(argv[1], "diff") == 0)
    {
        if (diff(argc, argv))
            return 1;
    }
    else
    {
        if (aliasCheck(argc, argv))
            return 1;
    }
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
    mkdir(".gigit\\branches");
    FILE *branches = fopen(".gigit\\branches\\currentBranch.txt", "w");
    fprintf(branches, "master");
    fclose(branches);
    FILE *id = fopen(".gigit\\branches\\id.txt", "w");
    fprintf(id, "0");
    fclose(id);
    FILE *lastbranch = fopen(".gigit\\branches\\lastbranch.txt", "w");
    fprintf(lastbranch, "master");
    fclose(lastbranch);
    closedir(dir);
    printf("Initialized empty gigit repository", path);
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
        char *aliasPath = malloc(MAX_PATH);
        if (!global)
        {
            DIR *dir = opendir(".");
            if (!dir)
            {
                printf("Error: not a directory\n");
                return 1;
            }
            char *path = malloc(MAX_PATH);
            strcpy(path, dir->dd_name);

            if (gigitExists(path) == NULL)
            {
                printf("Not a gigit repository\n");
                closedir(dir);
                return 1;
            }
            aliasPath = strcat(path, "\\config\\alias.txt");
            closedir(dir);
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
    else if (argc == 3 && strcmp(argv[2], "-n"))
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
                char *path = malloc(MAX_PATH);
                strcpy(path, dir->dd_name);
                char *path2 = malloc(sizeof(path) + sizeof("\\.gigit") + 1);
                strcpy(path2, path);
                char *gigitpath = malloc(MAX_PATH);
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
                    printf("malloc failed");
                    return 1;
                }
                strcpy(savePath, gigitpath);
                strcat(savePath, "\\stage");
                strcat(savePath, relativePath);
                mkdirRecursive(savePath);
                struct dirent *entry;
                while ((entry = readdir(dir)) != NULL)
                {
                    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".gigit") != 0)
                    {
                        deleteStar(path);
                        char *newPath = malloc(strlen(path) + strlen(entry->d_name) + 2);
                        strcpy(newPath, path);
                        strcat(newPath, "\\");

                        strcat(newPath, entry->d_name);
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
                char *path = malloc(strlen(fullPath) + 1);
                strcpy(path, fullPath);
                strrchr(path, '\\')[0] = '\0';
                char *gigitpath = gigitExists(path);
                if (gigitpath == NULL)
                {
                    printf("Not a gigit repository\n");
                    return 1;
                }
                char *relativePath = path + (strlen(gigitpath) - strlen("\\.gigit"));
                char *savePath = malloc(strlen(gigitpath) + strlen("\\stage") + strlen(relativePath) + 1);
                if (savePath == NULL)
                {
                    printf("malloc failed");
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
                    printf("malloc failed");
                    return 1;
                }
                char *temp = malloc(strlen(fullPath) + 1);
                if (temp == NULL)
                {
                    printf("malloc failed");
                    return 1;
                }
                strcpy(temp, fullPath);
                temp = temp + strlen(gigitpath) - strlen("\\.gigit");
                strcpy(lastPath, savePath);
                mkdirRecursive(lastPath);
                strcat(lastPath, temp);
                if (access(lastPath, F_OK) == 0)
                {
                    printf("file already added\n");
                    return 1;
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
        return 0;
    }

    else if (argc > 3 && !strcmp(argv[2], "-f"))
    {
        for (int i = 3; i < argc; i++)
        {
            add(3, (char *[]){"gigit", "add", argv[i]});
        }
    }
    else if (argc == 3 && !strcmp(argv[2], "-n"))
    {
        add(4, (char *[]){"gigit", "add", "-n", "1"});
        return 0;
    }
    else if (argc == 4 && !strcmp(argv[2], "-n"))
    {
        depth(argc, argv, 0);
    }
    else
    {
        printf("Invalid command\n");
        return 1;
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
    if (access(path, F_OK) == 0)
    {
        return 0;
    }
    char *path2 = malloc(strlen(path) + 1);
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

int reset(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Invalid command\n");
        return 1;
    }
    if (argc == 3 && strcmp(argv[2], "undo") != 0)
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
                char *path = malloc(MAX_PATH);
                strcpy(path, dir->dd_name);
                closedir(dir);
                deleteStar(path);
                char *path2 = malloc(strlen(path) + sizeof("\\.gigit") + 1);
                strcpy(path2, path);
                char *gigitpath = gigitExists(path2);
                free(path2);
                if (gigitpath == NULL)
                {
                    printf("Not a gigit repository\n");
                    return 1;
                }
                char *relativePath = path + strlen(gigitpath) - strlen("\\.gigit");
                char *savePath = malloc(strlen(gigitpath) + strlen("\\stage") + strlen(relativePath) + 1);
                if (savePath == NULL)
                {
                    printf("malloc failed");
                    return 1;
                }
                strcpy(savePath, gigitpath);
                strcat(savePath, "\\stage");
                strcat(savePath, relativePath);
                deleteStar(savePath);
                if (access(savePath, F_OK) != 0)
                {
                    printf("directory not added\n");
                    return 1;
                }
                DIR *dir2 = opendir(savePath);
                struct dirent *entry;
                while ((entry = readdir(dir2)) != NULL)
                {
                    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".gigit") != 0)
                    {
                        deleteStar(path);
                        char *newPath = strcat(path, "\\");
                        newPath = strcat(newPath, entry->d_name);
                        reset(3, (char *[]){"gigit", "reset", newPath});
                    }
                }
                closedir(dir2);
                rmdir(savePath);
                free(path);
                return 0;
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
                char *path = malloc(strlen(fullPath) + 1);
                strcpy(path, fullPath);
                strrchr(path, '\\')[0] = '\0';
                char *gigitpath = gigitExists(path);
                if (gigitpath == NULL)
                {
                    printf("Not a gigit repository\n");
                    return 1;
                }
                char *relativePath = path + (strlen(gigitpath) - strlen("\\.gigit"));
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
                strcat(lastPath, temp);
                if (access(lastPath, F_OK) != 0)
                {
                    printf("file not added\n");
                    return 1;
                }
                remove(lastPath);
            }
            return 0;
        }
    }
    else if (argc > 3 && !strcmp("-f", argv[2]))
    {
        for (int i = 3; i < argc; i++)
        {
            reset(3, (char *[]){"gigit", "reset", argv[i]});
        }
    }
    else
    {
        printf("Invalid command\n");
        return 1;
    }
}

int commit(int argc, char *argv[])
{
    if (argc == 3)
    {
        printf("No commit message\n");
        return 1;
    }
    else if (argc != 4 || strcmp(argv[2], "-m"))
    {
        printf("Invalid command\n");
        return 1;
    }
    else if (strlen(argv[3]) > 72)
    {
        printf("commit message too long\n");
        return 1;
    }
    // commit like git commit -m "message"
    else if (argc == 4 && !strcmp(argv[2], "-m"))
    {
        DIR *dir = opendir(".");
        if (!dir)
        {
            printf("Error: not a directory\n");
            return 1;
        }
        char *path = malloc(MAX_PATH);
        strcpy(path, dir->dd_name);
        deleteStar(path);
        char *path2 = malloc(strlen(path) + sizeof("\\.gigit") + 1);
        strcpy(path2, path);
        char *gigitpath = gigitExists(path2);

        // for getting branch name
        char *currentBranchPath = malloc(strlen(gigitpath) + strlen("\\branches\\currentBranch.txt") + 1);
        strcpy(currentBranchPath, gigitpath);
        strcat(currentBranchPath, "\\branches\\currentBranch.txt");
        FILE *currentBranch = fopen(currentBranchPath, "r");
        char branchName[100];
        fgets(branchName, 100, currentBranch);
        branchName[strlen(branchName)] = '\0';
        char *branchPath = malloc(strlen(gigitpath) + strlen("\\branches\\") + strlen(branchName) + 1);
        strcpy(branchPath, gigitpath);
        strcat(branchPath, "\\branches\\");
        strcat(branchPath, branchName);
        mkdir(branchPath);

        // for getting id
        char *idPath = malloc(strlen(branchPath) + strlen("\\id.txt") + 1);
        strcpy(idPath, gigitpath);
        strcat(idPath, "\\branches\\");
        strcat(idPath, "id.txt");
        FILE *idFile = fopen(idPath, "r");
        int id;
        char *idstr = malloc(100);
        fscanf(idFile, "%d", &id);
        itoa(id, idstr, 10);
        idstr[strlen(idstr)] = '\0';
        fclose(idFile);
        FILE *idFile2 = fopen(idPath, "w");
        fprintf(idFile2, "%d", id + 1);
        fclose(idFile2);

        // for getting username
        char *usernamePath = malloc(strlen(gigitpath) + strlen("\\config\\username.txt") + 1);
        strcpy(usernamePath, gigitpath);
        strcat(usernamePath, "\\config\\username.txt");
        FILE *usernameFile = fopen(usernamePath, "r");
        char username[100];
        fgets(username, 100, usernameFile);
        username[strlen(username)] = '\0';
        fclose(usernameFile);

        // for getting useremail
        char *useremailPath = malloc(strlen(gigitpath) + strlen("\\config\\useremail.txt") + 1);
        strcpy(useremailPath, gigitpath);
        strcat(useremailPath, "\\config\\useremail.txt");
        FILE *useremailFile = fopen(useremailPath, "r");
        char useremail[100];
        fgets(useremail, 100, useremailFile);
        useremail[strlen(useremail)] = '\0';
        fclose(useremailFile);

        // for getting date
        char date[100];
        SYSTEMTIME t;
        GetLocalTime(&t);
        sprintf(date, "%d-%02d-%02d %02d:%02d:%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);

        // for getting commit message
        char *commitMessage = malloc(strlen(argv[3]) + 1);
        strcpy(commitMessage, argv[3]);

        // making the commit from branchPath
        char *commitPath = malloc(strlen(branchPath) + strlen("\\") + 3);
        strcpy(commitPath, branchPath);
        strcat(commitPath, "\\");
        strcat(commitPath, idstr);
        mkdir(commitPath);

        // writing information to commitPath
        char *messagePath = malloc(strlen(commitPath) + strlen("\\message.txt") + 1);
        strcpy(messagePath, commitPath);
        strcat(messagePath, "\\message.txt");
        FILE *messageFile = fopen(messagePath, "w");
        fprintf(messageFile, "%s", commitMessage);
        fclose(messageFile);

        char *usernamePath2 = malloc(strlen(commitPath) + strlen("\\username.txt") + 1);
        strcpy(usernamePath2, commitPath);
        strcat(usernamePath2, "\\username.txt");
        FILE *usernameFile2 = fopen(usernamePath2, "w");
        fprintf(usernameFile2, "%s", username);
        fclose(usernameFile2);

        char *useremailPath2 = malloc(strlen(commitPath) + strlen("\\useremail.txt") + 1);
        strcpy(useremailPath2, commitPath);
        strcat(useremailPath2, "\\useremail.txt");
        FILE *useremailFile2 = fopen(useremailPath2, "w");
        fprintf(useremailFile2, "%s", useremail);
        fclose(useremailFile2);

        char *datePath = malloc(strlen(commitPath) + strlen("\\date.txt") + 1);
        strcpy(datePath, commitPath);
        strcat(datePath, "\\date.txt");
        FILE *dateFile = fopen(datePath, "w");
        fprintf(dateFile, "%s", date);
        fclose(dateFile);

        // char *nextCommitPath = malloc(strlen(commitPath) + strlen("\\nextCommit.txt") + 1);
        // strcpy(nextCommitPath, commitPath);
        // strcat(nextCommitPath, "\\nextCommit.txt");
        // FILE *nextCommitFile = fopen(nextCommitPath, "w");
        // fclose(nextCommitFile);

        char *prevCommitPath = malloc(strlen(commitPath) + strlen("\\prevCommit.txt") + 1);
        strcpy(prevCommitPath, commitPath);
        strcat(prevCommitPath, "\\prevCommit.txt");
        FILE *prevCommitFile = fopen(prevCommitPath, "w");
        fclose(prevCommitFile);

        // for writing id in the commit in id.txt
        char *commitIdPath = malloc(strlen(commitPath) + strlen("\\id.txt") + 1);
        strcpy(commitIdPath, commitPath);
        strcat(commitIdPath, "\\id.txt");
        FILE *commitIdFile = fopen(commitIdPath, "w");
        fprintf(commitIdFile, "%d", id);
        fclose(commitIdFile);
        int prevId = 5;
        if (id != 0)
        {
            // search in its own branch to see if there is a branch that has nextcommit file empty
            // if there is, write the commit id in it
            // and we assume that there is always in that branch that has next commit file empty
            // so code it
            char *branchPath2 = malloc(strlen(gigitpath) + strlen("\\branches\\") + strlen(branchName) + 1);
            strcpy(branchPath2, gigitpath);
            strcat(branchPath2, "\\branches\\");
            strcat(branchPath2, branchName); // branch name like master
            DIR *dir2 = opendir(branchPath2);
            struct dirent *entry;
            while ((entry = readdir(dir2)) != NULL)
            {
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, "id.txt") != 0 && strcmp(entry->d_name, "currentBranch.txt"))
                {
                    char *commitPath2 = malloc(strlen(branchPath2) + strlen("\\") + strlen(entry->d_name) + 1);
                    strcpy(commitPath2, branchPath2);
                    strcat(commitPath2, "\\");
                    strcat(commitPath2, entry->d_name);
                    char *nextCommitPath2 = malloc(strlen(commitPath2) + strlen("\\nextCommit.txt") + 1);
                    strcpy(nextCommitPath2, commitPath2);
                    strcat(nextCommitPath2, "\\nextCommit.txt");

                    int commitId = atoi(entry->d_name);
                    if (commitId == id)
                    {
                        continue;
                    }
                    if (fopen(nextCommitPath2, "r") == NULL)
                    {
                        char *nextCommitPath3 = malloc(strlen(commitPath2) + strlen("\\nextCommit.txt") + 1);
                        strcpy(nextCommitPath3, commitPath2);
                        strcat(nextCommitPath3, "\\nextCommit.txt");
                        FILE *nextCommitFile3 = fopen(nextCommitPath3, "w");
                        fprintf(nextCommitFile3, "%d", id);
                        fclose(nextCommitFile3);
                        char *prevCommitPath2 = malloc(strlen(commitPath) + strlen("\\prevCommit.txt") + 1);
                        strcpy(prevCommitPath2, commitPath);
                        strcat(prevCommitPath2, "\\prevCommit.txt");
                        FILE *prevCommitFile2 = fopen(prevCommitPath2, "w");
                        fprintf(prevCommitFile2, "%s", entry->d_name);
                        fclose(prevCommitFile2);
                        prevId = atoi(entry->d_name);
                        break;
                    }
                }
            }
        }
        // copy files from last commit to commit
        char *commitFilesPath = malloc(strlen(commitPath) + strlen("\\files") + 1);
        strcpy(commitFilesPath, commitPath);
        strcat(commitFilesPath, "\\files");
        mkdir(commitFilesPath);
        if (id != 0)
        {
            char *prevIdstr = malloc(100);
            itoa(prevId, prevIdstr, 10);
            char *prevCommitPath = malloc(strlen(gigitpath) + strlen("\\branches\\") + strlen(branchName) + strlen("\\") + strlen(idstr) + strlen("\\files") + 1);
            strcpy(prevCommitPath, gigitpath);
            strcat(prevCommitPath, "\\branches\\");
            strcat(prevCommitPath, branchName);
            strcat(prevCommitPath, "\\");
            strcat(prevCommitPath, prevIdstr);
            strcat(prevCommitPath, "\\files");
            // copyAndPaste(prevCommitPath, commitFilesPath);
            char *cmd4 = malloc(strlen(prevCommitPath) + strlen("xcopy  /s /e") + strlen(commitFilesPath) + strlen(" > nul") + 1);
            strcpy(cmd4, "xcopy ");
            strcat(cmd4, prevCommitPath);
            strcat(cmd4, " ");
            strcat(cmd4, commitFilesPath);
            strcat(cmd4, " /s /e");
            system(cmd4);
        }
        // copy files from staged to commit
        char *stagedPath = malloc(strlen(gigitpath) + strlen("\\stage") + 1);
        strcpy(stagedPath, gigitpath);
        strcat(stagedPath, "\\stage");
        // copyAndPaste(stagedPath, commitFilesPath);
        char *cmd = malloc(strlen(stagedPath) + strlen("xcopy  /s /e") + strlen(commitFilesPath) + strlen(" > nul") + 1);
        strcpy(cmd, "xcopy ");
        strcat(cmd, stagedPath);
        strcat(cmd, " ");
        strcat(cmd, commitFilesPath);
        strcat(cmd, " /s /e");
        system(cmd);

        // delete stage files using windows cmd
        char *cmd2 = malloc(strlen(stagedPath) + strlen("del /s /q ") + strlen(" > nul") + 1);
        strcpy(cmd2, "del /s /q ");
        strcat(cmd2, stagedPath);
        strcat(cmd2, " > nul");
        system(cmd2);

        // delete stage directroies using windows cmd
        char *cmd3 = malloc(strlen(stagedPath) + strlen("rmdir /s /q ") + strlen(" > nul") + 1);
        strcpy(cmd3, "rmdir /s /q ");
        strcat(cmd3, stagedPath);
        strcat(cmd3, " > nul");
        system(cmd3);

        // printing commit it, date, and message
        printf("ID: %d Date: %s Message: %s\n", id, date, commitMessage);
        return 0;
    }
}

int depth(int argc, char *argv[], int count)
{
    // completion for later
    if (argc < 4)
    {
        printf("Invalid command\n");
        return 1;
    }
    if (argc == 4 && !strcmp(argv[2], "-n"))
    {
        struct stat s;
        if (stat(argv[3], &s) == 0)
        {
            if (S_ISDIR(s.st_mode))
            {
                DIR *dir = opendir(argv[3]);
                if (!dir)
                {
                    printf("Error: not a directory\n");
                    return 1;
                }
                char *path = malloc(MAX_PATH);
                strcpy(path, dir->dd_name);
                closedir(dir);
                deleteStar(path);
                char *path2 = malloc(strlen(path) + sizeof("\\.gigit"));
                strcpy(path2, path);
                char *gigitpath = gigitExists(path2);
                free(path2);
                if (gigitpath == NULL)
                {
                    printf("Not a gigit repository\n");
                    return 1;
                }
                char *relativePath = path + strlen(gigitpath) - strlen("\\.gigit");
                char *savePath = malloc(strlen(gigitpath) + strlen("\\stage") + strlen(relativePath) + 1);
                if (savePath == NULL)
                {
                    printf("malloc failed");
                    return 1;
                }
                strcpy(savePath, gigitpath);
                strcat(savePath, "\\stage");
                strcat(savePath, relativePath);
                deleteStar(savePath);
                if (access(savePath, F_OK) != 0)
                {
                    printf("directory not added\n");
                    return 1;
                }
                DIR *dir2 = opendir(savePath);
                struct dirent *entry;
                while ((entry = readdir(dir2)) != NULL)
                {
                    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".gigit") != 0)
                    {
                        deleteStar(path);
                        char *newPath = strcat(path, "\\");
                        newPath = strcat(newPath, entry->d_name);
                        depth(4, (char *[]){"gigit", "add", "-n", newPath}, count + 1);
                    }
                }
                closedir(dir2);
                rmdir(savePath);
                free(path);
                return 0;
            }
        }
    }
}

int branch(int argc, char *argv[])
{
    if (argc == 2)
    {
        // printing all branch names
        DIR *dir = opendir(".");
        if (!dir)
        {
            printf("Error: not a directory\n");
            return 1;
        }
        char *path = malloc(MAX_PATH);
        strcpy(path, dir->dd_name);
        deleteStar(path);
        char *path2 = malloc(strlen(path) + sizeof("\\.gigit"));
        strcpy(path2, path);
        char *gigitpath = gigitExists(path2);
        if (gigitpath == NULL)
        {
            printf("Not a gigit repository\n");
            return 1;
        }
        char *branchesPath = malloc(strlen(gigitpath) + strlen("\\branches") + 1);
        strcpy(branchesPath, gigitpath);
        strcat(branchesPath, "\\branches");

        closedir(dir);

        DIR *gigitDir = opendir(branchesPath);
        struct dirent *entry;
        while ((entry = readdir(gigitDir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, "currentBranch.txt") != 0 && strcmp(entry->d_name, "id.txt") != 0 && strcmp(entry->d_name, "lastbranch.txt") != 0)
            {
                printf("%s\n", entry->d_name);
            }
        }
        return 0;
        closedir(gigitDir);
    }
    if (argc == 3)
    {
        // creating a new branch
        // and we must see that branch already exists or not
        DIR *dir = opendir(".");
        if (!dir)
        {
            printf("Error: not a directory\n");
            return 1;
        }
        char *path = malloc(MAX_PATH);
        strcpy(path, dir->dd_name);
        deleteStar(path);
        char *path2 = malloc(strlen(path) + sizeof("\\.gigit"));
        strcpy(path2, path);
        char *gigitpath = gigitExists(path2);
        if (gigitpath == NULL)
        {
            printf("Not a gigit repository\n");
            return 1;
        }
        char *branchesPath = malloc(strlen(gigitpath) + strlen("\\branches") + 1);
        strcpy(branchesPath, gigitpath);
        strcat(branchesPath, "\\branches");
        closedir(dir);
        DIR *gigitDir = opendir(branchesPath);
        struct dirent *entry;
        while ((entry = readdir(gigitDir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, "currentBranch.txt") != 0 && strcmp(entry->d_name, "id.txt") != 0 && strcmp(entry->d_name, "lastbranch.txt") != 0)
            {
                if (!strcmp(entry->d_name, argv[2]))
                {
                    printf("branch already exists\n");
                    return 1;
                }
            }
        }
        closedir(gigitDir);
        char *branchPath = malloc(strlen(branchesPath) + strlen("\\") + strlen(argv[2]) + 1);
        strcpy(branchPath, branchesPath);
        strcat(branchPath, "\\");
        strcat(branchPath, argv[2]); // branchesPath\\argv[2]
        mkdir(branchPath);

        // get the last commit id
        char *lastCommitIdPath = malloc(strlen(branchesPath) + strlen("\\") + strlen("\\id.txt") + 1);
        strcpy(lastCommitIdPath, branchesPath);
        strcat(lastCommitIdPath, "\\");
        strcat(lastCommitIdPath, "id.txt");
        FILE *lastCommitIdFile = fopen(lastCommitIdPath, "r");
        int intLastCommitId;
        fscanf(lastCommitIdFile, "%d", &intLastCommitId);
        intLastCommitId--;
        char lastCommitId[100];
        itoa(intLastCommitId, lastCommitId, 10);
        lastCommitId[strlen(lastCommitId)] = '\0';
        fclose(lastCommitIdFile);

        struct dirent *entry2;
        DIR *gigitDir2 = opendir(branchesPath);
        while ((entry2 = readdir(gigitDir2)) != NULL)
        {
            printf("%s\n", entry2->d_name);
            if (strcmp(entry2->d_name, ".") != 0 && strcmp(entry2->d_name, "..") != 0 && strcmp(entry2->d_name, "currentBranch.txt") != 0 && strcmp(entry2->d_name, "id.txt") != 0 && strcmp(entry2->d_name, "lastbranch.txt") != 0)
            {
                struct dirent *entry3;
                char *branchPath2 = malloc(strlen(branchesPath) + strlen("\\") + strlen(entry2->d_name) + 1);
                strcpy(branchPath2, branchesPath);
                strcat(branchPath2, "\\");
                strcat(branchPath2, entry2->d_name); // branchesPath\\entry2->d_name
                DIR *branchDir = opendir(branchPath2);
                while ((entry3 = readdir(branchDir)) != NULL)
                {
                    if (!strcmp(entry3->d_name, lastCommitId))
                    {
                        // see if fake.txt exists in that commit its not original commit
                        bool fake = false;
                        char *fakePath = malloc(strlen(branchPath2) + strlen("\\") + strlen(entry3->d_name) + strlen("\\fake.txt") + 1);
                        strcpy(fakePath, branchPath2);
                        strcat(fakePath, "\\");
                        strcat(fakePath, entry3->d_name);
                        strcat(fakePath, "\\fake.txt");
                        if (fopen(fakePath, "r") != NULL)
                        {
                            fake = true;
                        }
                        if (fake == true)
                        {
                            continue;
                        }
                        // copy that commit to the new branch
                        char *commitPath = malloc(strlen(branchPath2) + strlen("\\") + strlen(entry3->d_name) + 1);
                        strcpy(commitPath, branchPath2);
                        strcat(commitPath, "\\");
                        strcat(commitPath, entry3->d_name);
                        char *commitPath2 = malloc(strlen(branchPath) + strlen("\\") + strlen(entry3->d_name) + 1);
                        strcpy(commitPath2, branchPath);
                        strcat(commitPath2, "\\");
                        strcat(commitPath2, entry3->d_name);
                        mkdir(commitPath2);

                        // copy all things from last commit to commit
                        char *cmd = malloc(strlen(commitPath) + strlen("xcopy  /s /e") + strlen(commitPath2) + strlen(" > nul") + 1);
                        strcpy(cmd, "xcopy ");
                        strcat(cmd, commitPath);
                        strcat(cmd, " ");
                        strcat(cmd, commitPath2);
                        strcat(cmd, " /s /e");
                        system(cmd);

                        // making fake.txt in destination so we know it isnt original
                        char *fakePath2 = malloc(strlen(commitPath2) + strlen("\\fake.txt") + 1);
                        strcpy(fakePath2, commitPath2);
                        strcat(fakePath2, "\\fake.txt");
                        FILE *fakeFile = fopen(fakePath2, "w");
                        fprintf(fakeFile, "fake");
                        fclose(fakeFile);
                    }
                }
            }
        }
    }
    else
    {
        printf("Invalid Input\n");
    }
}

int checkout(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Invalid Input\n");
        return 1;
    }
    DIR *dir = opendir(".");
    if (!dir)
    {
        printf("Error: not a directory\n");
        return 1;
    }
    char *path = malloc(MAX_PATH);
    strcpy(path, dir->dd_name);
    deleteStar(path);
    char *path2 = malloc(strlen(path) + sizeof("\\.gigit"));
    strcpy(path2, path);
    char *gigitpath = gigitExists(path2);
    if (gigitpath == NULL)
    {
        printf("Not a gigit repository\n");
        return 1;
    }
    char *branchesPath = malloc(strlen(gigitpath) + strlen("\\branches") + 1);
    strcpy(branchesPath, gigitpath);
    strcat(branchesPath, "\\branches");
    closedir(dir);
    DIR *gigitDir = opendir(branchesPath);
    struct dirent *entry;
    bool branchExists = false;
    bool idExists = false;
    while ((entry = readdir(gigitDir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, "currentBranch.txt") != 0 && strcmp(entry->d_name, "id.txt") != 0 && strcmp(entry->d_name, "lastbranch.txt") != 0)
        {
            if (!strcmp(entry->d_name, argv[2]))
            {
                char *currentBranchPath = malloc(strlen(branchesPath) + strlen("\\currentBranch.txt") + 1);
                strcpy(currentBranchPath, branchesPath);
                strcat(currentBranchPath, "\\currentBranch.txt");
                FILE *currentBranchFile = fopen(currentBranchPath, "w");
                fprintf(currentBranchFile, "%s", argv[2]);
                fclose(currentBranchFile);
                branchExists = true;
                break;
            }
            struct dirent *entry2;
            char *branchPath2 = malloc(strlen(branchesPath) + strlen("\\") + strlen(entry->d_name) + 1);
            strcpy(branchPath2, branchesPath);
            strcat(branchPath2, "\\");
            strcat(branchPath2, entry->d_name); // branchesPath\\entry->d_name
            DIR *branchDir = opendir(branchPath2);
            while ((entry2 = readdir(branchDir)) != NULL)
            {
                if (strcmp(entry2->d_name, ".") != 0 && strcmp(entry2->d_name, "..") != 0 && strcmp(entry2->d_name, "id.txt") != 0)
                {
                    if (!strcmp(entry2->d_name, argv[2]))
                    {
                        bool fake = false;
                        char *fakePath = malloc(strlen(branchPath2) + strlen("\\") + strlen(entry2->d_name) + strlen("\\fake.txt") + 1);
                        strcpy(fakePath, branchPath2);
                        strcat(fakePath, "\\");
                        strcat(fakePath, entry2->d_name);
                        strcat(fakePath, "\\fake.txt");
                        if (fopen(fakePath, "r") != NULL)
                        {
                            fake = true;
                        }
                        if (fake == true)
                        {
                            continue;
                        }
                        idExists = true;
                        char *currentBranchFile = malloc(strlen(branchesPath) + strlen("\\currentBranch.txt") + 1);
                        strcpy(currentBranchFile, branchesPath);
                        strcat(currentBranchFile, "\\currentBranch.txt");
                        FILE *currentBranchFile2 = fopen(currentBranchFile, "w");
                        fprintf(currentBranchFile2, "%s", entry->d_name);
                        fclose(currentBranchFile2);
                        break;
                    }
                }
            }
        }
    }
    if (!strcmp(argv[2], "HEAD"))
        branchExists = true;
    if (branchExists == false && idExists == false)
    {
        printf("branch doesn't exists\n");
        return 1;
    }
    // delete all the files in the directory and copy the files from the last commit of branch to the directory
    char *currentBranchPath = malloc(strlen(branchesPath) + strlen("\\currentBranch.txt") + 1);
    strcpy(currentBranchPath, branchesPath);
    strcat(currentBranchPath, "\\currentBranch.txt");
    FILE *currentBranchFile = fopen(currentBranchPath, "r");
    char currentBranch[100];
    fgets(currentBranch, 100, currentBranchFile);
    currentBranch[strlen(currentBranch)] = '\0';

    char *currentproject = malloc(strlen(gigitpath) + 1);
    strcpy(currentproject, gigitpath);
    strrchr(currentproject, '\\')[0] = '\0';

    // delete the things inside currentproject path
    struct dirent *entry3;
    DIR *dir3 = opendir(currentproject);
    while ((entry3 = readdir(dir3)) != NULL)
    {
        if (strcmp(entry3->d_name, ".") != 0 && strcmp(entry3->d_name, "..") != 0 && strcmp(entry3->d_name, ".gigit") != 0)
        {
            char *deletePath = malloc(strlen(currentproject) + strlen("\\") + strlen(entry3->d_name) + 1);
            strcpy(deletePath, currentproject);
            strcat(deletePath, "\\");
            strcat(deletePath, entry3->d_name);
            // remove using windows cmd
            char *cmd = malloc(strlen(deletePath) + strlen("del /s /q") + strlen(" > nul") + 1);
            strcpy(cmd, "del /s /q ");
            strcat(cmd, deletePath);
            strcat(cmd, " > nul");
            system(cmd);

            // remove using rmdir
            char *cmd2 = malloc(strlen(deletePath) + strlen("rmdir /s /q") + strlen(" > nul") + 1);
            strcpy(cmd2, "rmdir /s /q ");
            strcat(cmd2, deletePath);
            strcat(cmd2, " > nul");
            system(cmd2);
        }
    }

    // copy the files from the last commit of branch to the directory
    // last id doesnt have nextCommit.txt
    char *lastCommitIdPath = malloc(strlen(branchesPath) + strlen("\\") + strlen(currentBranch) + 1);
    strcpy(lastCommitIdPath, branchesPath);
    strcat(lastCommitIdPath, "\\");
    strcat(lastCommitIdPath, currentBranch);
    struct dirent *entry4;
    DIR *dir4 = opendir(lastCommitIdPath);
    while ((entry4 = readdir(dir4)) != NULL)
    {
        if (strcmp(entry4->d_name, ".") != 0 && strcmp(entry4->d_name, "..") != 0 && strcmp(entry4->d_name, "id.txt") != 0)
        {
            char *commitPath = malloc(strlen(lastCommitIdPath) + strlen("\\") + strlen(entry4->d_name) + 1);
            strcpy(commitPath, lastCommitIdPath);
            strcat(commitPath, "\\");
            strcat(commitPath, entry4->d_name);
            char *commitFilesPath = malloc(strlen(commitPath) + strlen("\\nextCommit.txt") + 1);
            strcpy(commitFilesPath, commitPath);
            strcat(commitFilesPath, "\\nextCommit.txt");
            if (fopen(commitFilesPath, "r") != NULL)
            {
                continue;
            }

            char *cmd = malloc(strlen(commitPath) + strlen("xcopy  /s /e") + strlen(currentproject) + strlen(" > nul") + 1);
            strcpy(cmd, "xcopy ");
            strcat(cmd, commitPath);
            strcat(cmd, " ");
            strcat(cmd, currentproject);
            strcat(cmd, " /s /e");
            system(cmd);
        }
    }
}

int log1(int argc, char *argv[])
{
    if (argc == 2)
    {
        DIR *dir = opendir(".");
        if (!dir)
        {
            printf("Error: not a directory\n");
            return 1;
        }
        char *path = malloc(MAX_PATH);
        strcpy(path, dir->dd_name);
        deleteStar(path);
        char *path2 = malloc(strlen(path) + sizeof("\\.gigit"));
        strcpy(path2, path);
        char *gigitpath = gigitExists(path2);
        if (gigitpath == NULL)
        {
            printf("Not a gigit repository\n");
            return 1;
        }
        char *idPath = malloc(strlen(gigitpath) + strlen("\\branches\\id.txt") + 1);
        strcpy(idPath, gigitpath);
        strcat(idPath, "\\branches\\id.txt");
        FILE *idFile = fopen(idPath, "r");
        int id;
        fscanf(idFile, "%d", &id);
        fclose(idFile);
        for (int i = id - 1; i >= 0; i--)
        {
            logID(i);
        }
    }
    // now we check log -n where n is a number
    else if (argc == 4 && !strcmp(argv[2], "-n"))
    {
        int n = atoi(argv[3]);
        DIR *dir = opendir(".");
        if (!dir)
        {
            printf("Error: not a directory\n");
            return 1;
        }
        char *path = malloc(MAX_PATH);
        strcpy(path, dir->dd_name);
        deleteStar(path);
        char *path2 = malloc(strlen(path) + sizeof("\\.gigit"));
        strcpy(path2, path);
        char *gigitpath = gigitExists(path2);
        if (gigitpath == NULL)
        {
            printf("Not a gigit repository\n");
            return 1;
        }
        char *idPath = malloc(strlen(gigitpath) + strlen("\\branches\\id.txt") + 1);
        strcpy(idPath, gigitpath);
        strcat(idPath, "\\branches\\id.txt");
        FILE *idFile = fopen(idPath, "r");
        int id;
        fscanf(idFile, "%d", &id);
        fclose(idFile);
        for (int i = id - 1; i >= id - n; i--)
        {
            logID(i);
        }
    }
    else if (argc == 4 && !strcmp(argv[2], "-branch"))
    {
        // logID(id) from branch in order of biggest id to smallest id
        // first see if branch name exists
        DIR *dir = opendir(".");
        if (!dir)
        {
            printf("Error: not a directory\n");
            return 1;
        }
        char *path = malloc(MAX_PATH);
        strcpy(path, dir->dd_name);
        deleteStar(path);
        char *path2 = malloc(strlen(path) + sizeof("\\.gigit"));
        strcpy(path2, path);
        char *gigitpath = gigitExists(path2);
        if (gigitpath == NULL)
        {
            printf("Not a gigit repository\n");
            return 1;
        }
        char *branchesPath = malloc(strlen(gigitpath) + strlen("\\branches") + 1);
        strcpy(branchesPath, gigitpath);
        strcat(branchesPath, "\\branches");
        closedir(dir);
        DIR *gigitDir = opendir(branchesPath);
        struct dirent *entry;
        bool branchExists = false;
        while ((entry = readdir(gigitDir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, "currentBranch.txt") != 0 && strcmp(entry->d_name, "id.txt") != 0 && strcmp(entry->d_name, "lastbranch.txt") != 0)
            {
                if (!strcmp(entry->d_name, argv[3]))
                {
                    branchExists = true;
                    break;
                }
            }
        }
        if (branchExists == false)
        {
            printf("branch doesn't exists\n");
            return 1;
        }
        // a commit doest have id.txt file
        struct dirent *entry2;
        DIR *gigitDir2 = opendir(branchesPath);
        int id[100];
        int n = 0;
        while ((entry2 = readdir(gigitDir2)) != NULL)
        {
            if (strcmp(entry2->d_name, ".") != 0 && strcmp(entry2->d_name, "..") != 0 && strcmp(entry2->d_name, "currentBranch.txt") != 0 && strcmp(entry2->d_name, "id.txt") != 0 && strcmp(entry2->d_name, "lastbranch.txt") != 0)
            {
                if (!strcmp(entry2->d_name, argv[3]))
                {
                    struct dirent *entry3;
                    char *branchPath2 = malloc(strlen(branchesPath) + strlen("\\") + strlen(entry2->d_name) + 1);
                    strcpy(branchPath2, branchesPath);
                    strcat(branchPath2, "\\");
                    strcat(branchPath2, entry2->d_name); // branchesPath\\entry2->d_name
                    DIR *branchDir = opendir(branchPath2);
                    while ((entry3 = readdir(branchDir)) != NULL)
                    {
                        if (strcmp(entry3->d_name, ".") != 0 && strcmp(entry3->d_name, "..") != 0 && strcmp(entry3->d_name, "id.txt") != 0)
                        {
                            id[n] = atoi(entry3->d_name);
                            n++;
                        }
                    }
                }
            }
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n - i - 1; j++)
            {
                if (id[j] < id[j + 1])
                {
                    int temp = id[j];
                    id[j] = id[j + 1];
                    id[j + 1] = temp;
                }
            }
        }
        for (int i = 0; i < n; i++)
        {
            logID(id[i]);
        }
    }
    else if (argc == 4 && !strcmp(argv[2], "-since") || !strcmp(argv[2], "-before"))
    {

        bool datePrinted = false;
        DIR *dir = opendir(".");
        if (!dir)
        {
            printf("Error: not a directory\n");
            return 1;
        }
        char *path = malloc(MAX_PATH);
        strcpy(path, dir->dd_name);
        deleteStar(path);
        char *path2 = malloc(strlen(path) + sizeof("\\.gigit"));
        strcpy(path2, path);
        char *gigitpath = gigitExists(path2);
        if (gigitpath == NULL)
        {
            printf("Not a gigit repository\n");
            return 1;
        }
        char *branchesPath = malloc(strlen(gigitpath) + strlen("\\branches") + 1);
        strcpy(branchesPath, gigitpath);
        strcat(branchesPath, "\\branches");
        closedir(dir);
        DIR *gigitDir = opendir(branchesPath);
        struct dirent *entry;
        int id[100];
        int n = 0;
        while ((entry = readdir(gigitDir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, "currentBranch.txt") != 0 && strcmp(entry->d_name, "id.txt") != 0 && strcmp(entry->d_name, "lastbranch.txt") != 0)
            {
                struct dirent *entry2;
                char *branchPath2 = malloc(strlen(branchesPath) + strlen("\\") + strlen(entry->d_name) + 1);
                strcpy(branchPath2, branchesPath);
                strcat(branchPath2, "\\");
                strcat(branchPath2, entry->d_name); // branchesPath\\entry->d_name
                DIR *branchDir = opendir(branchPath2);
                while ((entry2 = readdir(branchDir)) != NULL)
                {
                    if (strcmp(entry2->d_name, ".") != 0 && strcmp(entry2->d_name, "..") != 0 && strcmp(entry2->d_name, "id.txt") != 0)
                    {
                        char *commitPath = malloc(strlen(branchPath2) + strlen("\\") + strlen(entry2->d_name) + 1);
                        strcpy(commitPath, branchPath2);
                        strcat(commitPath, "\\");
                        strcat(commitPath, entry2->d_name);
                        char *datePath = malloc(strlen(commitPath) + strlen("\\date.txt") + 1);
                        strcpy(datePath, commitPath);
                        strcat(datePath, "\\date.txt");
                        FILE *dateFile = fopen(datePath, "r");
                        char date[100];
                        fgets(date, 100, dateFile);
                        date[strlen(date)] = '\0';
                        fclose(dateFile);
                        if (strcmp(date, argv[3]) > 0 && !strcmp(argv[2], "-since"))
                        {
                            logID(atoi(entry2->d_name));
                            datePrinted = true;
                        }
                        else if (strcmp(date, argv[3]) < 0 && !strcmp(argv[2], "-before"))
                        {
                            logID(atoi(entry2->d_name));
                            datePrinted = true;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n - i - 1; j++)
            {
                if (id[j] < id[j + 1])
                {
                    int temp = id[j];
                    id[j] = id[j + 1];
                    id[j + 1] = temp;
                }
            }
        }
        for (int i = 0; i < n; i++)
        {
            logID(id[i]);
        }
        if (datePrinted == false)
        {
            printf("No commits found\n");
        }
    }
    else if (argc == 4 && !strcmp(argv[2], "-search"))
    {
        // search through the messages of the commit to see if they are same to argv[3]
        // if they are same print it
        bool messagePrinted = false;
        DIR *dir = opendir(".");
        if (!dir)
        {
            printf("Error: not a directory\n");
            return 1;
        }
        char *path = malloc(MAX_PATH);
        strcpy(path, dir->dd_name);
        deleteStar(path);
        char *path2 = malloc(strlen(path) + sizeof("\\.gigit"));
        strcpy(path2, path);
        char *gigitpath = gigitExists(path2);
        if (gigitpath == NULL)
        {
            printf("Not a gigit repository\n");
            return 1;
        }
        char *branchesPath = malloc(strlen(gigitpath) + strlen("\\branches") + 1);
        strcpy(branchesPath, gigitpath);
        strcat(branchesPath, "\\branches");
        closedir(dir);
        DIR *gigitDir = opendir(branchesPath);
        struct dirent *entry;
        int id[100];
        int n = 0;
        while ((entry = readdir(gigitDir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, "currentBranch.txt") != 0 && strcmp(entry->d_name, "id.txt") != 0 && strcmp(entry->d_name, "lastbranch.txt") != 0)
            {
                struct dirent *entry2;
                char *branchPath2 = malloc(strlen(branchesPath) + strlen("\\") + strlen(entry->d_name) + 1);
                strcpy(branchPath2, branchesPath);
                strcat(branchPath2, "\\");
                strcat(branchPath2, entry->d_name); // branchesPath\\entry->d_name
                DIR *branchDir = opendir(branchPath2);
                while ((entry2 = readdir(branchDir)) != NULL)
                {
                    if (strcmp(entry2->d_name, ".") != 0 && strcmp(entry2->d_name, "..") != 0 && strcmp(entry2->d_name, "id.txt") != 0)
                    {
                        char *commitPath = malloc(strlen(branchPath2) + strlen("\\") + strlen(entry2->d_name) + 1);
                        strcpy(commitPath, branchPath2);
                        strcat(commitPath, "\\");
                        strcat(commitPath, entry2->d_name);
                        char *messagePath = malloc(strlen(commitPath) + strlen("\\message.txt") + 1);
                        strcpy(messagePath, commitPath);
                        strcat(messagePath, "\\message.txt");
                        FILE *messageFile = fopen(messagePath, "r");
                        char message[100];
                        fgets(message, 100, messageFile);
                        message[strlen(message)] = '\0';
                        fclose(messageFile);
                        if (strstr(message, argv[3]) != NULL)
                        {
                            id[n] = atoi(entry2->d_name);
                            n++;
                            messagePrinted = true;
                        }
                    }
                }
            }
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n - i - 1; j++)
            {
                if (id[j] < id[j + 1])
                {
                    int temp = id[j];
                    id[j] = id[j + 1];
                    id[j + 1] = temp;
                }
            }
        }
        for (int i = 0; i < n; i++)
        {
            logID(id[i]);
        }
        if (messagePrinted == false)
        {
            printf("No commits found\n");
        }
    }
    else
    {
        printf("Invalid Input\n");
        return 1;
    }
    return 0;
}

int logID(int ID)
{
    DIR *dir = opendir(".");
    if (!dir)
    {
        printf("Error: not a directory\n");
        return 1;
    }
    char *path = malloc(MAX_PATH);
    strcpy(path, dir->dd_name);
    deleteStar(path);
    char *path2 = malloc(strlen(path) + sizeof("\\.gigit"));
    strcpy(path2, path);
    char *gigitpath = gigitExists(path2);
    if (gigitpath == NULL)
    {
        printf("Not a gigit repository\n");
        return 1;
    }
    char *branchesPath = malloc(strlen(gigitpath) + strlen("\\branches") + 1);
    strcpy(branchesPath, gigitpath);
    strcat(branchesPath, "\\branches");
    struct dirent *entry;
    DIR *gigitDir = opendir(branchesPath);
    while ((entry = readdir(gigitDir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, "currentBranch.txt") != 0 && strcmp(entry->d_name, "id.txt") != 0 && strcmp(entry->d_name, "lastbranch.txt") != 0)
        {
            struct dirent *entry2;
            char *branchPath2 = malloc(strlen(branchesPath) + strlen("\\") + strlen(entry->d_name) + 1);
            strcpy(branchPath2, branchesPath);
            strcat(branchPath2, "\\");
            strcat(branchPath2, entry->d_name); // branchesPath\\entry->d_name
            DIR *branchDir = opendir(branchPath2);
            while ((entry2 = readdir(branchDir)) != NULL)
            {
                if (strcmp(entry2->d_name, ".") != 0 && strcmp(entry2->d_name, "..") != 0 && strcmp(entry2->d_name, "id.txt") != 0)
                {
                    if (atoi(entry2->d_name) == ID)
                    {
                        // check fake.txt to see its fake or not
                        bool fake = false;
                        char *fakePath = malloc(strlen(branchPath2) + strlen("\\") + strlen(entry2->d_name) + strlen("\\fake.txt") + 1);
                        strcpy(fakePath, branchPath2);
                        strcat(fakePath, "\\");
                        strcat(fakePath, entry2->d_name);
                        strcat(fakePath, "\\fake.txt");
                        if (fopen(fakePath, "r") != NULL)
                        {
                            fake = true;
                        }
                        if (fake == true)
                        {
                            continue;
                        }
                        char *commitPath = malloc(strlen(branchPath2) + strlen("\\") + strlen(entry2->d_name) + 1);
                        strcpy(commitPath, branchPath2);
                        strcat(commitPath, "\\");
                        strcat(commitPath, entry2->d_name);

                        // printing date of commit
                        char *datePath = malloc(strlen(commitPath) + strlen("\\date.txt") + 1);
                        strcpy(datePath, commitPath);
                        strcat(datePath, "\\date.txt");
                        FILE *dateFile = fopen(datePath, "r");
                        char date[100];
                        fgets(date, 100, dateFile);
                        date[strlen(date)] = '\0';
                        fclose(dateFile);
                        printf("Commit Date: %s\n", date);

                        // printing message of commit
                        char *messagePath = malloc(strlen(commitPath) + strlen("\\message.txt") + 1);
                        strcpy(messagePath, commitPath);
                        strcat(messagePath, "\\message.txt");
                        FILE *messageFile = fopen(messagePath, "r");
                        char message[100];
                        fgets(message, 100, messageFile);
                        message[strlen(message)] = '\0';
                        fclose(messageFile);
                        printf("Commit Message: %s\n", message);

                        // printing username of commit
                        char *usernamePath = malloc(strlen(commitPath) + strlen("\\username.txt") + 1);
                        strcpy(usernamePath, commitPath);
                        strcat(usernamePath, "\\username.txt");
                        FILE *usernameFile = fopen(usernamePath, "r");
                        char username[100];
                        fgets(username, 100, usernameFile);
                        username[strlen(username)] = '\0';
                        fclose(usernameFile);
                        printf("Username: %s", username);

                        printf("Commit ID: %d\n", ID);

                        // printing branch of commit
                        printf("Commit Branch: %s\n", entry->d_name);

                        // printing how many files are in commit
                        char *filesPath = malloc(strlen(commitPath) + strlen("\\files") + 1);
                        strcpy(filesPath, commitPath);
                        strcat(filesPath, "\\files");
                        char command[256];
                        sprintf(command, "cmd /c \"cd /d %s && (dir /a-d /b ^| find /c /v \"\"\")\"", filesPath);
                        int count = system(command);
                        printf("Number of files: %d\n\n", count);
                    }
                }
            }
        }
    }
}

int tag(int argc, char *argv[])
{
    if (argc > 2 && !strcmp(argv[2], "-a"))
    {
        bool messageExists = false;
        char message[100];
        bool IdExists = false;
        int ID;
        bool overwrite = false;

        char *tagName = malloc(strlen(argv[3]));
        strcpy(tagName, argv[3]);

        for (int i = 4; i < argc; i++)
        {
            if (!strcmp(argv[i], "-m"))
            {
                messageExists = true;
                strcpy(message, argv[i + 1]);
            }
            if (!strcmp(argv[i], "-c"))
            {
                IdExists = true;
                ID = atoi(argv[i + 1]);
            }
            if (!strcmp(argv[i], "-f"))
            {
                overwrite = true;
            }
        }

        DIR *dir = opendir(".");
        if (!dir)
        {
            printf("Error: not a directory\n");
            return 1;
        }
        char *path = malloc(MAX_PATH);
        strcpy(path, dir->dd_name);
        deleteStar(path);
        char *path2 = malloc(strlen(path) + sizeof("\\.gigit"));
        strcpy(path2, path);
        char *gigitpath = gigitExists(path2);
        if (gigitpath == NULL)
        {
            printf("Not a gigit repository\n");
            return 1;
        }
        char *tagsPath = malloc(strlen(gigitpath) + strlen("\\tags") + 1);
        strcpy(tagsPath, gigitpath);
        strcat(tagsPath, "\\tags");
        closedir(dir);
        mkdir(tagsPath);

        // check if tag already exists
        char *tagPath = malloc(strlen(tagsPath) + strlen("\\") + strlen(tagName) + strlen(".txt") + 1);
        strcpy(tagPath, tagsPath);
        strcat(tagPath, "\\");
        strcat(tagPath, tagName);
        strcat(tagPath, ".txt");
        if (fopen(tagPath, "r") != NULL)
        {
            if (overwrite == false)
            {
                printf("tag already exists\n");
                return 1;
            }
        }
        // write the tag
        FILE *tagFile = fopen(tagPath, "w");
        fprintf(tagFile, "Tag name: %s\n", tagName);
        if (IdExists == true)
        {
            fprintf(tagFile, "Commit ID: %d\n", ID);
        }
        else
        {
            // find the last commit id in current branch
            char *branchesPath = malloc(strlen(gigitpath) + strlen("\\branches") + 1);
            strcpy(branchesPath, gigitpath);
            strcat(branchesPath, "\\branches");
            char *currentBranchPath = malloc(strlen(branchesPath) + strlen("\\currentBranch.txt") + 1);
            strcpy(currentBranchPath, branchesPath);
            strcat(currentBranchPath, "\\currentBranch.txt");
            FILE *currentBranchFile = fopen(currentBranchPath, "r");
            char currentBranch[100];
            fgets(currentBranch, 100, currentBranchFile);
            currentBranch[strlen(currentBranch)] = '\0';
            fclose(currentBranchFile);
            char *lastCommitIdPath = malloc(strlen(branchesPath) + strlen("\\") + strlen(currentBranch) + 1);
            strcpy(lastCommitIdPath, branchesPath);
            strcat(lastCommitIdPath, "\\");
            strcat(lastCommitIdPath, currentBranch);
            struct dirent *entry;
            DIR *dir = opendir(lastCommitIdPath);
            while ((entry = readdir(dir)) != NULL)
            {
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, "id.txt") != 0)
                {
                    char *nextCommitPath = malloc(strlen(lastCommitIdPath) + strlen("\\") + strlen(entry->d_name) + 1);
                    strcpy(nextCommitPath, lastCommitIdPath);
                    strcat(nextCommitPath, "\\");
                    strcat(nextCommitPath, entry->d_name);
                    char *nextCommitPath2 = malloc(strlen(nextCommitPath) + strlen("\\nextCommit.txt") + 1);
                    strcpy(nextCommitPath2, nextCommitPath);
                    strcat(nextCommitPath2, "\\nextCommit.txt");
                    if (fopen(nextCommitPath2, "r") == NULL)
                    {
                        ID = atoi(entry->d_name);
                        fprintf(tagFile, "Commit ID: %d\n", ID);
                        break;
                    }
                }
            }
        }
        // convert ID into str
        char *IDstr = malloc(100);
        itoa(ID, IDstr, 10);

        // write the author
        // find the username of the ID commit usint username.txt and usernamePath
        // find the current branch
        char *branchesPath = malloc(strlen(gigitpath) + strlen("\\branches") + 1);
        strcpy(branchesPath, gigitpath);
        strcat(branchesPath, "\\branches");
        char *currentBranchPath = malloc(strlen(branchesPath) + strlen("\\currentBranch.txt") + 1);
        strcpy(currentBranchPath, branchesPath);
        strcat(currentBranchPath, "\\currentBranch.txt");
        FILE *currentBranchFile = fopen(currentBranchPath, "r");
        char currentBranch[100];
        fgets(currentBranch, 100, currentBranchFile);
        currentBranch[strlen(currentBranch)] = '\0';
        fclose(currentBranchFile);
        char *lastCommitIdPath = malloc(strlen(branchesPath) + strlen("\\") + strlen(currentBranch) + strlen("\\") + 3);
        strcpy(lastCommitIdPath, branchesPath);
        strcat(lastCommitIdPath, "\\");
        strcat(lastCommitIdPath, currentBranch);
        strcat(lastCommitIdPath, "\\");
        strcat(lastCommitIdPath, IDstr);
        char *usernamePath = malloc(strlen(lastCommitIdPath) + strlen("\\username.txt") + 1);
        strcpy(usernamePath, lastCommitIdPath);
        strcat(usernamePath, "\\username.txt");
        FILE *usernameFile = fopen(usernamePath, "r");
        char username[100];
        fgets(username, 100, usernameFile);
        username[strlen(username)] = '\0';
        fclose(usernameFile);
        // get the user email
        char *userEmailPath = malloc(strlen(lastCommitIdPath) + strlen("\\useremail.txt") + 1);
        strcpy(userEmailPath, lastCommitIdPath);
        strcat(userEmailPath, "\\useremail.txt");
        FILE *userEmailFile = fopen(userEmailPath, "r");
        char useremail[100];
        fgets(useremail, 100, usernameFile);
        useremail[strlen(useremail)] = '\0';
        fclose(usernameFile);

        fprintf(tagFile, "Author: %s <%s>\n", username, useremail);
        // write the date
        char *datePath = malloc(strlen(lastCommitIdPath) + strlen("\\date.txt") + 1);
        strcpy(datePath, lastCommitIdPath);
        strcat(datePath, "\\date.txt");
        FILE *dateFile = fopen(datePath, "r");
        char date[100];
        fgets(date, 100, dateFile);
        date[strlen(date)] = '\0';
        fclose(dateFile);
        fprintf(tagFile, "Date: %s\n", date);
        // write the message
        if (messageExists == true)
        {
            fprintf(tagFile, "Message: %s\n", message);
        }
    }
    else if (argc == 2)
    {
        DIR *dir = opendir(".");
        if (!dir)
        {
            printf("Error: not a directory\n");
            return 1;
        }
        char *path = malloc(MAX_PATH);
        strcpy(path, dir->dd_name);
        deleteStar(path);
        char *path2 = malloc(strlen(path) + sizeof("\\.gigit"));
        strcpy(path2, path);
        char *gigitpath = gigitExists(path2);
        if (gigitpath == NULL)
        {
            printf("Not a gigit repository\n");
            return 1;
        }
        test char *tagsPath = malloc(strlen(gigitpath) + strlen("\\tags") + 1);
        strcpy(tagsPath, gigitpath);
        strcat(tagsPath, "\\tags");
        closedir(dir);
        DIR *gigitDir = opendir(tagsPath);
        struct dirent *entry;
        test while ((entry = readdir(gigitDir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                char *tagPath = malloc(strlen(tagsPath) + strlen("\\") + strlen(entry->d_name) + 1);
                strcpy(tagPath, tagsPath);
                strcat(tagPath, "\\");
                strcat(tagPath, entry->d_name);
                FILE *tagFile = fopen(tagPath, "r");
                char tag[100];
                while (fgets(tag, 100, tagFile))
                {
                    printf("%s", tag);
                }
                printf("\n");
                fclose(tagFile);
            }
        }
        return 0;
    }
    else if (argc == 4 && !strcmp(argv[2], "show"))
    {
        // neogit tag show <tag-name>
        char *tagName = malloc(strlen(argv[3]) + 1);
        strcpy(tagName, argv[3]);
        DIR *dir = opendir(".");
        if (!dir)
        {
            printf("Error: not a directory\n");
            return 1;
        }
        char *path = malloc(MAX_PATH);
        strcpy(path, dir->dd_name);
        deleteStar(path);
        char *path2 = malloc(strlen(path) + sizeof("\\.gigit"));
        strcpy(path2, path);
        char *gigitpath = gigitExists(path2);
        if (gigitpath == NULL)
        {
            printf("Not a gigit repository\n");
            return 1;
        }
        char *tagsPath = malloc(strlen(gigitpath) + strlen("\\tags") + 1);
        strcpy(tagsPath, gigitpath);
        strcat(tagsPath, "\\tags");
        closedir(dir);
        char *tagPath = malloc(strlen(tagsPath) + strlen("\\") + strlen(tagName) + strlen(".txt") + 1);
        strcpy(tagPath, tagsPath);
        strcat(tagPath, "\\");
        strcat(tagPath, tagName);
        strcat(tagPath, ".txt");
        FILE *tagFile = fopen(tagPath, "r");
        char tag[100];
        while (fgets(tag, 100, tagFile))
        {
            printf("%s", tag);
        }
        printf("\n");
        fclose(tagFile);
        return 0;
    }
    else
    {
        printf("Invalid Input\n");
        return 1;
    }
}

int status(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Invalid Input\n");
        return 1;
    }

    DIR *dir = opendir(".");
    if (!dir)
    {
        printf("Error: not a directory\n");
        return 1;
    }
    char *path = malloc(MAX_PATH);
    strcpy(path, dir->dd_name);
    deleteStar(path);
    char *path2 = malloc(strlen(path) + sizeof("\\.gigit"));
    strcpy(path2, path);
    char *gigitpath = gigitExists(path2);
    if (gigitpath == NULL)
    {
        printf("Not a gigit repository\n");
        return 1;
    }
    char *stagedPath = malloc(strlen(gigitpath) + strlen("\\staged") + 1);
    strcpy(stagedPath, gigitpath);
    strcat(stagedPath, "\\stage");
    closedir(dir);
    char *startPath = malloc(strlen(gigitpath) + 1);
    strcpy(startPath, gigitpath);
    strrchr(startPath, '\\')[0] = '\0';
    printstatus(startPath, stagedPath);
    return 0;
}

void printstatus(char *path, char *stagedPath)
{
    // see if files inside the path are staged or not and write a recursive function to check files inside directories
    struct dirent *entry;
    DIR *dir = opendir(path);
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".gigit") != 0)
        {
            char *path2 = malloc(strlen(path) + strlen("\\") + strlen(entry->d_name) + 1);
            strcpy(path2, path);
            strcat(path2, "\\");
            strcat(path2, entry->d_name);
            char *stagedPath2 = malloc(strlen(stagedPath) + strlen("\\") + strlen(entry->d_name) + 1);
            strcpy(stagedPath2, stagedPath);
            strcat(stagedPath2, "\\");
            strcat(stagedPath2, entry->d_name);
            // first see if stagedPath2 exists and its a file
            bool pathIsFile = false;
            struct stat path_stat;
            stat(path2, &path_stat);
            if (S_ISREG(path_stat.st_mode))
            {
                pathIsFile = true;
            }
            if (fopen(stagedPath2, "r") != NULL && pathIsFile == true)
            {
                printf("+ %s\n", entry->d_name);
            }
            else if (pathIsFile == true)
            {
                printf("- %s\n", entry->d_name);
            }
            else if (pathIsFile == false)
            {
                printstatus(path2, stagedPath2);
            }
        }
    }
    return;
}

int grep(int argc, char *argv[])
{
    if (argc < 6)
    {
        printf("Invalid Input\n");
        return 1;
    }
    if (strcmp(argv[2], "-f") || strcmp(argv[4], "-p"))
    {
        printf("Invalid Input\n");
        return 1;
    }
    bool commitExists = false;
    int ID;
    bool nExists = false;
    char *file = malloc(MAX_PATH);
    char *word = malloc(MAX_PATH);
    for (int i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i], "-c"))
        {
            commitExists = true;
            ID = atoi(argv[i + 1]);
        }
        if (!strcmp(argv[i], "-n"))
        {
            nExists = true;
        }
        if (!strcmp(argv[i], "-f"))
        {
            strcpy(file, argv[i + 1]);
        }
        if (!strcmp(argv[i], "-p"))
        {
            strcpy(word, argv[i + 1]);
        }
    }
    // search for the word in the file in the commit
    DIR *dir = opendir(".");
    if (!dir)
    {
        printf("Error: not a directory\n");
        return 1;
    }
    char *path = malloc(MAX_PATH);
    strcpy(path, dir->dd_name);
    deleteStar(path);
    char *path2 = malloc(strlen(path) + sizeof("\\.gigit"));
    strcpy(path2, path);
    char *gigitpath = gigitExists(path2);
    if (gigitpath == NULL)
    {
        printf("Not a gigit repository\n");
        return 1;
    }
    if (!commitExists)
    {
        char *branchesPath = malloc(strlen(gigitpath) + strlen("\\branches") + 1);
        strcpy(branchesPath, gigitpath);
        strcat(branchesPath, "\\branches");
        char *currentBranchPath = malloc(strlen(branchesPath) + strlen("\\currentBranch.txt") + 1);
        strcpy(currentBranchPath, branchesPath);
        strcat(currentBranchPath, "\\currentBranch.txt");
        FILE *currentBranchFile = fopen(currentBranchPath, "r");
        char currentBranch[100];
        fgets(currentBranch, 100, currentBranchFile);
        currentBranch[strlen(currentBranch)] = '\0';
        fclose(currentBranchFile);
        char *lastCommitIdPath = malloc(strlen(branchesPath) + strlen("\\") + strlen(currentBranch) + 1);
        strcpy(lastCommitIdPath, branchesPath);
        strcat(lastCommitIdPath, "\\");
        strcat(lastCommitIdPath, currentBranch);
        struct dirent *entry;
        DIR *dir = opendir(lastCommitIdPath);
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, "id.txt") != 0)
            {
                char *nextCommitPath = malloc(strlen(lastCommitIdPath) + strlen("\\") + strlen(entry->d_name) + 1);
                strcpy(nextCommitPath, lastCommitIdPath);
                strcat(nextCommitPath, "\\");
                strcat(nextCommitPath, entry->d_name);
                char *nextCommitPath2 = malloc(strlen(nextCommitPath) + strlen("\\nextCommit.txt") + 1);
                strcpy(nextCommitPath2, nextCommitPath);
                strcat(nextCommitPath2, "\\nextCommit.txt");
                if (fopen(nextCommitPath2, "r") == NULL)
                {
                    ID = atoi(entry->d_name);
                    break;
                }
            }
        }
    }
    // find the commit path using ID
    // make the ID str
    char *branchesPath = malloc(strlen(gigitpath) + strlen("\\branches") + 1);
    strcpy(branchesPath, gigitpath);
    strcat(branchesPath, "\\branches");
    char *currentBranchPath = malloc(strlen(branchesPath) + strlen("\\currentBranch.txt") + 1);
    strcpy(currentBranchPath, branchesPath);
    strcat(currentBranchPath, "\\currentBranch.txt");
    FILE *currentBranchFile = fopen(currentBranchPath, "r");
    char currentBranch[100];
    fgets(currentBranch, 100, currentBranchFile);
    currentBranch[strlen(currentBranch)] = '\0';
    fclose(currentBranchFile);

    char *IDstr = malloc(100);
    itoa(ID, IDstr, 10);
    char *commitPath = malloc(strlen(branchesPath) + strlen("\\") + strlen(currentBranch) + strlen("\\") + strlen(IDstr) + 1);
    strcpy(commitPath, branchesPath);
    strcat(commitPath, "\\");
    strcat(commitPath, currentBranch);
    strcat(commitPath, "\\");
    strcat(commitPath, IDstr);
    char *commitFilesPath = malloc(strlen(commitPath) + strlen("\\files") + 1);
    strcpy(commitFilesPath, commitPath);
    strcat(commitFilesPath, "\\files");

    // find the file in the commit
    char *file2 = fileFinding(commitFilesPath, file);
    if (file2 == NULL)
    {
        printf("File not found in the commit\n");
        return 1;
    }
    // search for the word in the file
    FILE *fileFile = fopen(file2, "r");
    char line[100];
    int lineNo = 1;
    while (fgets(line, 100, fileFile) != NULL)
    {
        if (strstr(line, word) != NULL)
        {
            if (nExists == true)
            {
                printf("%d: %s", lineNo, line);
            }
            else
            {
                printf("%s", line);
            }
        }
        lineNo++;
    }

    return 0;
}

char *fileFinding(char *path, char *file)
{
    struct dirent *entry;
    DIR *dir = opendir(path);
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            // see its a file or directory
            struct stat path_stat;
            char *path2 = malloc(strlen(path) + strlen("\\") + strlen(entry->d_name) + 1);
            strcpy(path2, path);
            strcat(path2, "\\");
            strcat(path2, entry->d_name);
            struct stat path_stat2;
            stat(path2, &path_stat2);
            if (S_ISDIR(path_stat2.st_mode))
            {
                fileFinding(path2, file);
            }
            else
            {
                if (!strcmp(entry->d_name, file))
                {
                    return path2;
                }
            }
        }
    }
    return NULL;
}

int preCommit(int argc, char *argv[])
{
    if (!strcmp(argv[2], "hooks") && !strcmp(argv[3], "list"))
    {
        char *hooksPath = "C:\\Users\\Admin\\OneDrive\\Desktop\\SUT\\gigit\\gigit\\hooks";
        struct dirent *entry;
        DIR *dir = opendir(hooksPath);
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                char *hookPath = malloc(strlen(hooksPath) + strlen("\\") + strlen(entry->d_name) + 1);
                strcpy(hookPath, hooksPath);
                strcat(hookPath, "\\");
                strcat(hookPath, entry->d_name);
                FILE *hook = fopen(hookPath, "r");
                char line[100];
                while (fgets(line, 100, hook) != NULL)
                {
                    printf("%s", line);
                }
                printf("\n\n");
            }
        }
        return 0;
    }

    else if (!strcmp(argv[2], "applied") && !strcmp(argv[3], "hooks"))
    {
        DIR *dir = opendir(".");
        if (!dir)
        {
            printf("Error: not a directory\n");
            return 1;
        }
        char *path = malloc(MAX_PATH);
        strcpy(path, dir->dd_name);
        deleteStar(path);
        char *path2 = malloc(strlen(path) + sizeof("\\.gigit"));
        strcpy(path2, path);
        char *gigitpath = gigitExists(path2);
        if (gigitpath == NULL)
        {
            printf("Not a gigit repository\n");
            return 1;
        }
        char *hooksPath = malloc(strlen(gigitpath) + strlen("\\hooks") + 1);
        strcpy(hooksPath, gigitpath);
        strcat(hooksPath, "\\hooks");
        mkdir(hooksPath);
        closedir(dir);
        DIR *dir2 = opendir(hooksPath);
        struct dirent *entry;
        bool sthPrinted = false;
        while ((entry = readdir(dir2)) != NULL)
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                sthPrinted = true;
                char *hookPath = malloc(strlen(hooksPath) + strlen("\\") + strlen(entry->d_name) + 1);
                strcpy(hookPath, hooksPath);
                strcat(hookPath, "\\");
                strcat(hookPath, entry->d_name);
                FILE *hook = fopen(hookPath, "r");
                char line[100];
                while (fgets(line, 100, hook) != NULL)
                {
                    printf("%s", line);
                }
                printf("\n\n");
            }
        }
        if (sthPrinted == false)
        {
            printf("No hooks applied\n");
            return 0;
        }
        return 0;
    }

    else if (!strcmp(argv[2], "add") && !strcmp(argv[3], "hook"))
    {
        char *hookId = malloc(strlen(argv[4]) + 1);
        strcpy(hookId, argv[4]);
        char *hooksPath = "C:\\Users\\Admin\\OneDrive\\Desktop\\SUT\\gigit\\gigit\\hooks";
        struct dirent *entry;
        DIR *dir = opendir(hooksPath);
        bool hookExists = false;
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && !strcmp(entry->d_name, hookId))
            {
                hookExists = true;
                break;
            }
        }
        if (hookExists == false)
        {
            printf("Hook not found\n");
            return 1;
        }
        DIR *dir2 = opendir(".");
        if (!dir2)
        {
            printf("Error: not a directory\n");
            return 1;
        }
        char *path = malloc(MAX_PATH);
        strcpy(path, dir2->dd_name);
        deleteStar(path);
        char *path2 = malloc(strlen(path) + sizeof("\\.gigit") + 1);
        strcpy(path2, path);
        char *gigitpath = gigitExists(path2);
        if (gigitpath == NULL)
        {
            printf("Not a gigit repository\n");
            return 1;
        }
        char *hooksPath2 = malloc(strlen(gigitpath) + strlen("\\hooks") + 1);
        strcpy(hooksPath2, gigitpath);
        strcat(hooksPath2, "\\hooks");
        closedir(dir2);
        char *hookPath = malloc(strlen(hooksPath) + strlen("\\") + strlen(hookId) + 1);
        strcpy(hookPath, hooksPath);
        strcat(hookPath, "\\");
        strcat(hookPath, hookId);
        char *hookPath2 = malloc(strlen(hooksPath2) + strlen("\\") + strlen(hookId) + 1);
        strcpy(hookPath2, hooksPath2);
        strcat(hookPath2, "\\");
        strcat(hookPath2, hookId);
        FILE *hookFile = fopen(hookPath, "r");
        FILE *hookFile2 = fopen(hookPath2, "w");
        char line[100];
        while (fgets(line, 100, hookFile) != NULL)
        {
            fprintf(hookFile2, "%s", line);
        }
        fclose(hookFile);
        fclose(hookFile2);
        return 0;
    }

    else if (!strcmp(argv[2], "remove") && !strcmp(argv[3], "hook"))
    {
        char *hookId = malloc(strlen(argv[4]) + 1);
        strcpy(hookId, argv[4]);
        DIR *dir = opendir(".");
        if (!dir)
        {
            printf("Error: not a directory\n");
            return 1;
        }
        char *path = malloc(MAX_PATH);
        strcpy(path, dir->dd_name);
        deleteStar(path);
        char *path2 = malloc(strlen(path) + sizeof("\\.gigit") + 1);
        strcpy(path2, path);
        char *gigitpath = gigitExists(path2);
        if (gigitpath == NULL)
        {
            printf("Not a gigit repository\n");
            return 1;
        }
        char *hooksPath = malloc(strlen(gigitpath) + strlen("\\hooks\\") + strlen(hookId) + 1);
        strcpy(hooksPath, gigitpath);
        strcat(hooksPath, "\\hooks\\");
        strcat(hooksPath, hookId);
        if (fopen(hooksPath, "r") == NULL)
        {
            printf("Hook not found\n");
            return 1;
        }
        else
        {
            remove(hooksPath);
            return 0;
        }
    }
    else
    {
        printf("Invalid Input\n");
        return 1;
    }
}

int diff(int argc, char *argv[])
{
    // gigit diff -f <file1> <file2> –line1 <begin-end> –line2 <begin-end>
    FILE *file1 = fopen(argv[3], "r");
    FILE *file2 = fopen(argv[4], "r");
    if (file1 == NULL || file2 == NULL)
    {
        printf("File not found\n");
        return 1;
    }
    bool line1Exists = false;
    bool line2Exists = false;
    int line1Begin;
    int line1End;
    int line2Begin;
    int line2End;
    for (int i = 5; i < argc; i++)
    {
        if (!strcmp(argv[i], "-line1"))
        {
            char *line1 = malloc(strlen(argv[i + 1]) + 1);
            strcpy(line1, argv[i + 1]);
            line1Begin = atoi(strtok(line1, "-"));
            line1End = atoi(strtok(NULL, "-"));
            line1Exists = true;
        }
        if (!strcmp(argv[i], "-line2"))
        {
            char *line2 = malloc(strlen(argv[i + 1]) + 1);
            strcpy(line2, argv[i + 1]);
            line2Begin = atoi(strtok(line2, "-"));
            line2End = atoi(strtok(NULL, "-"));
            line2Exists = true;
        }
    }
    char line1[100];
    char line2[100];
    int line1No = 1;
    int line2No = 1;
    if (line1Exists)
    {
        for (int i = 1; i < line1Begin; i++)
        {
            fgets(line1, 100, file1);
            line1No++;
        }
    }
    if (line2Exists)
    {
        for (int i = 1; i < line2Begin; i++)
        {
            fgets(line2, 100, file2);
            line2No++;
        }
    }
    while (fgets(line1, 100, file1) != NULL && fgets(line2, 100, file2) != NULL)
    {
        if (line1Exists && line1No > line1End)
        {
            break;
        }
        if (line2Exists && line2No > line2End)
        {
            break;
        }
        char *line1Copy = malloc(101);
        strcpy(line1Copy, line1);
        while (strtok(line1Copy, "\n \t") == NULL)
        {
            fgets(line1, 100, file1);
            if (line1 == NULL)
            {
                break;
            }
            line1No++;
            strcpy(line1Copy, line1);
        }
        char *line2Copy = malloc(101);
        strcpy(line2Copy, line2);
        while (strtok(line2Copy, "\n \t") == NULL)
        {
            fgets(line2, 100, file2);
            if (line2 == NULL)
            {
                break;
            }
            line2No++;
            strcpy(line2Copy, line2);
        }
        line1[strlen(line1) - 1] = '\0';
        line2[strlen(line2) - 1] = '\0';
        if (strcmp(line1, line2) != 0)
        {
            printf("<<<<\n");
            printf("%s-%d\n", argv[3], line1No);
            printf("%s\n", line1);
            printf("%s-%d\n", argv[4], line2No);
            printf("%s\n", line2);
            printf(">>>>\n");
        }
        line1No++;
        line2No++;
    }

    return 0;
}
