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


int main() {
        char date[100];
        SYSTEMTIME t;
        GetLocalTime(&t);
        sprintf(date, "%d-%02d-%02d %02d:%02d:%02d", t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
        printf("%s\n", date);
}