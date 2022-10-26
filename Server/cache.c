#include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>

char* readFile(char* filename){
    FILE *file = fopen(filename, "rb");
    long lSize;
    char *buffer;

    fseek( file , 0L , SEEK_END);
    lSize = ftell( file );
    rewind( file );

    buffer = calloc( 1, lSize+1 );

    if( !buffer )
        fclose(file),
        fputs("memory alloc fails",stderr),
        exit(1);

    
    if( 1!=fread( buffer , lSize, 1 , file) ) 
        fclose(file),
        free(buffer),
        fputs("entire read fails",stderr),
        exit(1);

    fclose(file);

    return buffer;
}

char *rmstr (char *str, const char *chars) {
    const char *c = chars;          
    while (*c) {                    
        char *p = str;              
        while (*p) {                
            if (*p == *c) {         
                char *sp = p,       
                    *ep = p + 1;    
                do
                    *sp++ = *ep;    
                while (*ep++);     
            }
            p++;                    
        }
        c++;                        
    }
    return str;    
}

char *createFileName(char* petition){
    char* cacheFolder = "Server/cache/";

    // Cutting Petition
    char * petitionHead;
    petitionHead = strtok(petition, "\r");

    // Deleting spaces, / and .
    rmstr(petitionHead, " ");
    rmstr(petitionHead, "/");
    rmstr(petitionHead, ".");

    // Creating filename
    char* fileName;
    fileName = malloc(strlen(cacheFolder)+1+strlen(petitionHead));
    strcpy(fileName, cacheFolder); 
    strcat(fileName, petitionHead);

    return fileName;
}

char *sgets(char *s, int n, const char **strp){
    if(**strp == '\0')return NULL;
    int i;
    for(i=0;i<n-1;++i, ++(*strp)){
        s[i] = **strp;
        if(**strp == '\0')
            break;
        if(**strp == '\n'){
            s[i+1]='\0';
            ++(*strp);
            break;
        }
    }
    if(i==n-1)
        s[i] = '\0';
    return s;
}

void saveResponse(char *petition, char *response){

    char *fileName = createFileName(petition);
    
    if (access(fileName, F_OK) != 0) {
        // file doesn't exist
        FILE *file = fopen(fileName, "a");

        // Copying response to responsecpy
        const char *responsecpy = response;
        char buff[800];
        const char **p = &responsecpy;

        boolean isResponse = 0;
        fputs("\n", file);fputs("\n", file);
        while(NULL!=sgets(buff, sizeof(buff), p)){
            if(buff[0]=='<') isResponse = 1;

            if(!isResponse){
                buff[strlen(buff)-1] = '\0';
            } else {
                fputs(buff, file);
            }
        }

        fclose(file);
    } 
}

char *getResponse(char *petition){
    char *fileName = createFileName(petition);
    
    if (access(fileName, F_OK) == 0) {
       return readFile(fileName);
    } else {
       return "\r\n\r\n<h1>There is an error</h1>\n";
    }      
}

void clearCache(){
    mkdir("Server/cache");
    DIR *cacheFolder = opendir("Server/cache");
    struct dirent *next_file;

    while ((next_file = readdir(cacheFolder)) != NULL){
        char fileName[200] = "Server/cache/";
        strcat(fileName, next_file->d_name);
        remove(fileName);
    }

    closedir(cacheFolder);
}

void checkCacheTimeout(){

    // Current time
    time_t currentTime;
    time ( &currentTime );
    double diff = INT_MAX;

    DIR *cacheFolder = opendir("Server/cache");
    struct dirent *next_file;
   
    while ((next_file = readdir(cacheFolder)) != NULL){
        char fileName[200] = "Server/cache/";
        strcat(fileName, next_file->d_name);
        
        // File creation time
        struct stat filestat;
        stat(fileName, &filestat);

        // Difference in seconds 
        double newdiff = difftime(currentTime, filestat.st_mtime);
        if(newdiff < diff) diff = newdiff;
    }

    closedir(cacheFolder);

    //  Differece between times more tha 20 secs
    if(diff > 20.0){
        clearCache();
    }
}