/*
  serverManager.c
  Server

  Created by Mário Jorge Gabriel da Silva on 16/06/13.
  Copyright (c) 2013 Mário Jorge Gabriel da Silva. All rights reserved.
*/

#include "serverManager.h"

pBackbone BackboneGlobal = NULL;

int createPipe()
{
    int isRunning;
    isRunning = access(npServer, R_OK);
    if(isRunning != -1)
    {
        printf("Central's already running!\n");
        return 0;
    }
    
    mkfifo(npServer,0644);
    
    if((access(npServer,W_OK)) == -1)
    {
        printf("Error: Cannot create Central's named pipe!\n");
        return 0;
    }
    return 1;
}


int serversUp()
{
    pOutbound in = malloc(sizeof(Outbound));
    pInbound out = malloc(sizeof(Inbound));
    int inbox;
    pUsers tempUser = NULL;
    int tempDay;
    
    BackboneGlobal = loadBackBone();
    tempUser=loadTerminalUsers(BackboneGlobal);
    tempDay=loadDayVar(BackboneGlobal);
    
    if(BackboneGlobal == NULL || tempUser == NULL || tempDay == 0)
    {
        printf("Config files are missing!\n");
        printf("Shutting down..\n");
        exit(0);
    }
    
    printf("Creating Central's named pipe..\n");
    if(!createPipe())
        exit (0);
    
    inbox = open(npServer, O_RDWR);
    
    signal(SIGUSR1, shutdownTT);
    
    do {
        fflush(stdout);
        read(inbox,&in->len,sizeof(unsigned long));
        read(inbox,&in->pid,sizeof(int));
        read(inbox,&in->userLevel,sizeof(int));
        read(inbox,&in->jobRef,sizeof(int));
        if(in->userLevel == 1)
        {
            read(inbox,&in->pw,sizeof(char)*20);
        }
        if(in->userLevel == 2 && in->jobRef == 1)
        {
            read(inbox,&in->username,sizeof(char)*20);
            read(inbox,&in->pw,sizeof(char)*20);
        }
        if(in->len != 0)
        {
            read(inbox,&in->nArgs,sizeof(int));
            in->args = malloc(sizeof(in->len));
            read(inbox,in->args,sizeof(char)*in->len);
        }
        
        if(in->userLevel == 1)  /* Level 1 - Admin */ 
        {
            if(in->jobRef == 1)
            {
                if(checkLoginA(BackboneGlobal, in->pid, in->pw))
                    out->valid = 1;
                else
                    out->valid = 0;
                writePipe(npAdmin,out);
            }
            else
            {
                if(checkLoginA(BackboneGlobal, in->pid, in->pw))
                    findTaskA(in->jobRef, in->nArgs, in->args, in->pid, in->len, BackboneGlobal);
            }
        } else if(in->userLevel == 2){  /* Level 2 - User */
           if(in->jobRef == 1)
            {
                checkLoginT(BackboneGlobal, in->pid, in->username, in->pw);
            } else if(checkPidTerm(BackboneGlobal, in->pid))
                    findTaskT(in->jobRef, in->nArgs, in->args, in->pid, in->len, BackboneGlobal);
                    else
                        kill(in->pid, SIGUSR1);
        } else {   /* Level 3 - Logger */ 
                if(in->jobRef == 1)
                    loggersUp(BackboneGlobal,in->pid);
                else
                    loggersDown(BackboneGlobal);
        } 
    } while (1);
    
    free(in);
    free(out);
    free(tempUser);
    in = NULL;
    out = NULL;
    tempUser = NULL;
    
    return 0;
}