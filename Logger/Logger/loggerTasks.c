#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>

#include "ioLBound.h"
#include "varConf.h"


void writeTemp(char *filename)
{
    FILE *f;
    f = fopen(TEMP, "w");
    fprintf(f,"%s",filename);
    fclose(f);
}

void writeLastLine(char *filename)
{
    FILE *f;
    f = fopen(filename, "a");
    
    fprintf(f,"---------------------------------\n");
    fclose(f);
}

int writetxtFile(pInbound inAux, char *filename)
{
    FILE *f;
    f = fopen(filename, "a");

    if(f==NULL)
    {
        printf("Error: log file!\n");
        return 0;
    }

    fprintf(f,"%s\n",inAux->report);
    fclose(f);
    return 1;
}

void outputs(pInbound inAux, char *filename)
{
    printf("%s\n", inAux->report);
    writetxtFile(inAux,filename);
    
}

int createPipe()
{
    int isRunning;
    isRunning = access(npLog, R_OK);
    if(isRunning != -1)
    {
        printf("Logger session already in use!\n");
        return 0;
    }
    
    mkfifo(npLog,0644);
    
    if((access(npLog,W_OK)) == -1)
    {
        printf("Error: Creating Logger Pipe.\n");
        return 0;
    }
    
    if((access(npServer,R_OK)) == -1)
    {
        printf("Central is not running at the moment!\n");
        return 0;
    }
    
    return 1;
}

int writePipe(pOutbound out)
{
    int outbox;
    
    outbox = open(npServer, O_WRONLY);
    if((access(npServer,R_OK)) == -1)
    {
        return 0;
    }
    
    fflush(stdout);
    write(outbox,&out->len,sizeof(unsigned long));
    write(outbox,&out->pid,sizeof(int));
    write(outbox,&out->userLevel,sizeof(int));
    write(outbox,&out->jobRef,sizeof(int));
    close(outbox);
    return 1;
}

void shutdownLog()
{
    FILE *f;
    char filename[20];
    
    int len=0;
    int job=2;
    int userlevel=3;
    int pid=getpid();
    
    pOutbound out = malloc(sizeof(Outbound));
    
    out->len = len;
    out->pid = pid;
    out->userLevel = userlevel;
    out->jobRef = job;
    
    f = fopen(TEMP, "r");
    fscanf(f,"%s",filename);
    
    writeLastLine(filename);
    writePipe(out);
    
    printf("Central is shutting down!\n");
    free(out);
    out = NULL;
    unlink(npLog);
    unlink(TEMP);
    exit(0);
}

int alertServerUp(char *filename)
{
    int len=0;
    int job=1;
    int userlevel=3;
    int pid=getpid();
    
    pOutbound out = malloc(sizeof(Outbound));

    out->len = len;
    out->pid = pid;
    out->userLevel = userlevel;
    out->jobRef = job;

    if(!writePipe(out))
        return 0;
    else
        return 1;
    
}