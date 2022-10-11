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

void appendLog(const char *log){
    logFile = fopen ("Server/log/log.txt", "a");
    char *currentTime = getCurrentTime();
    fprintf(logFile, currentTime);
    fprintf(logFile, log);
    fprintf(logFile,"\n");
    fprintf(logFile,"\n");
    fclose(logFile);

    printf(currentTime);
    printf(log);
    printf("\n");
    printf("\n");
}