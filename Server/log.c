#include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
  
FILE *logFile;

char* getCurrentTime(){
    time_t now = time(NULL);
    return ctime(&now);    
}

void createLogFile(){
    mkdir("Server/log");
    logFile = fopen ("Server/log/log.txt", "a");
    fclose(logFile);
}

void appendLog(char *log){

    // Copying log to logcpy
    char *logcpy = malloc(strlen(log));
    memcpy(logcpy, log, strlen(log));
    strtok(logcpy, "\t\n");

    // Writing in log.txt
    logFile = fopen ("Server/log/log.txt", "a");
    char *currentTime = getCurrentTime();
    fprintf(logFile, currentTime);
    fprintf(logFile, logcpy, "\n");
    fprintf(logFile,"\n");
    fclose(logFile);

    // Printing
    printf(currentTime);
    printf(logcpy, "\n");
    printf("\n");
}