#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

#include "varConf.h"
#include "ioLBound.h"
#include "loggerTasks.h"

int loggersUp()
{
    pInbound in = malloc(sizeof(Inbound));
    int inbox;
    
    if(!createPipe())
        exit(0);
    
    writeTemp(LogFile);
    
    if(!alertServerUp(LogFile))
        exit(0);
    
    inbox = open(npLog, O_RDWR);
    
    
    signal(SIGUSR2, shutdownLog);
    
    do{
        fflush(stdout);
        read(inbox,&in->valid,sizeof(int));
        read(inbox,&in->len,sizeof(unsigned long));
        in->report = malloc(sizeof(in->len));
        read(inbox,in->report,sizeof(char)*in->len);

        if(in->valid == 1)
        {
            if(in->len > 0)
                outputs(in,LogFile);
        }
    } while(1);
    
    free(in);
    in = NULL;
}

int main()
{
    int pid;
    pid = fork();
    
    if (pid < 0)
        return 0;
    else
        if(pid !=0)
            exit(0);
        else
            loggersUp();
}

