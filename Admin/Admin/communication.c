/*
 communication.c
 Terminal
 
 Created by Mário Jorge Gabriel da Silva on 18/06/13.
 Copyright (c) 2013 Mário Jorge Gabriel da Silva. All rights reserved.
 */

#include "communication.h"

pInbound startCommunication(pOutbound req, int pid)
{
    fflush(stdout);
    pInbound in = malloc(sizeof(Inbound));
    int outbox, inbox;
    
    outbox = open(npServer, O_WRONLY);
    
    write(outbox,&req->len,sizeof(unsigned long));
    write(outbox,&req->pid,sizeof(int));
    write(outbox,&req->userLevel,sizeof(int));
    write(outbox,&req->jobRef,sizeof(int));
    if(req->userLevel == 1)
    {
        write(outbox,&req->pw,sizeof(char)*20);
    }
    if(req->len != 0)
    {
        write(outbox,&req->nArgs,sizeof(int));
        write(outbox,req->args,sizeof(char)*req->len);
    }
    close(outbox);
    

    inbox = open(npAdmin, O_RDONLY);
    read(inbox,in,sizeof(Inbound));
    close(inbox);
    return in;
}

pInboundExtended startCommunicationExtended(pOutbound req, int pid)
{
    fflush(stdout);
    pInboundExtended in = malloc(sizeof(InboundExtended));
    int outbox, inbox;
    
    
    outbox = open(npServer, O_WRONLY);
    
    write(outbox,&req->len,sizeof(unsigned long));
    write(outbox,&req->pid,sizeof(int));
    write(outbox,&req->userLevel,sizeof(int));
    write(outbox,&req->jobRef,sizeof(int));
    if(req->userLevel == 1)
    {
        write(outbox,&req->pw,sizeof(char)*20);
    }
    if(req->len != 0)
    {
        write(outbox,&req->nArgs,sizeof(int));
        write(outbox,req->args,sizeof(char)*req->len);
    }
    
    close(outbox);
    inbox = open(npAdmin, O_RDONLY);
    
    read(inbox,&in->valid,sizeof(int));
    read(inbox,&in->len,sizeof(unsigned long));
    in->report = malloc(sizeof(in->len));
    read(inbox,in->report,sizeof(char)*in->len);
    
    close(inbox);
    return in;
}