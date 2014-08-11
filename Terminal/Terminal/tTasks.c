//
//  tTasks.c
//  Terminal
//
//  Created by Mário Silva on 07/08/14.
//  Copyright (c) 2014 Mário Jorge Gabriel da Silva. All rights reserved.
//

#include "tTasks.h"

void shutdownTerm()
{
    printf("Central is shutting down!\n");
    exit(0);
}

void responseText(pInboundExtended inAux)
{
    char *sep;
    sep = strtok(inAux->report, "!");
    while(sep)
    {
        printf("%s\n", sep);
        sep = strtok(NULL, "!");
    }
}

int cancelFlight(char *extendedArgs, int pid)
{
    pOutbound request = malloc(sizeof(Outbound));
    pInboundExtended response;
    unsigned long len;
    len = strlen(extendedArgs)+1;
    
    request->len = len;
    request->pid = pid;
    request->userLevel = 2;
    request->jobRef = 7;
    request->nArgs = 2;
    request->args = extendedArgs;
    
    response = startCommunicationExtended(request,pid);
    
    free(request);
    request = NULL;
    
    if(response->valid == 1)
    {
        printf("Passport removed from the flight manifest\n");
        free(response);
        response = NULL;
        return 1;
    }
    else
    {
        printf("Error: cancel flight. Can not find Passport on the flight manifest\n");
        free(response);
        response = NULL;
        return 0;
    }
}

int bookFlight(char *extendedArgs, int pid)
{
    pOutbound request = malloc(sizeof(Outbound));
    pInboundExtended response;
    unsigned long len;
    len = strlen(extendedArgs)+1;
    
    request->len = len;
    request->pid = pid;
    request->userLevel = 2;
    request->jobRef = 6;
    request->nArgs = 2;
    request->args = extendedArgs;
    
    response = startCommunicationExtended(request,pid);
    
    free(request);
    request = NULL;
    
    if(response->valid == 1)
    {
        printf("Passport added to the flight manifest.\n");
        free(response);
        response = NULL;
        return 1;
    }
    else
    {
        printf("Error: book flight. Passport is already on the manifest?\n");
        free(response);
        response = NULL;
        return 0;
    }
}

int searchFlight(char *extendedArgs, int pid)
{
    pOutbound request = malloc(sizeof(Outbound));
    pInboundExtended response;
    unsigned long len;
    len = strlen(extendedArgs)+1;
    
    request->len = len;
    request->pid = pid;
    request->userLevel = 2;
    request->jobRef = 5;
    request->nArgs = 2;
    request->args = extendedArgs;
    
    response = startCommunicationExtended(request,pid);
    
    free(request);
    request = NULL;
    
    if(response->valid == 1)
    {
        responseText(response);
        free(response);
        response = NULL;
        return 1;
    } else
        printf("No flight available between these cities.\n");
    
    free(response);
    response = NULL;
    return 0;
}

int changePassword(char *extendedArgs, int pid)
{
    pOutbound request = malloc(sizeof(Outbound));
    pInbound response;
    unsigned long len;
    len = strlen(extendedArgs)+1;
    
    request->len = len;
    request->pid = pid;
    request->userLevel = 2;
    request->jobRef = 3;
    request->nArgs = 2;
    request->args = extendedArgs;
    
    response = startCommunication(request,pid);
    
    free(request);
    request = NULL;
    
    if(response->valid == 1)
    {
        printf("Done. Logout and then login.\n");
        free(response);
        response = NULL;
        return 1;
    } else {
        printf("Password verification failed!\n");
    }
    free(response);
    response = NULL;
    return 0;
}

int flightList(int pid)
{
    pOutbound request = malloc(sizeof(Outbound));
    pInboundExtended response;
    
    request->len = 0;
    request->pid = pid;
    request->userLevel = 2;
    request->jobRef = 4;
    
    response = startCommunicationExtended(request,pid);
    
    free(request);
    request = NULL;
    
    if(response->valid == 1)
    {
        if(response->len > 0)
            responseText(response);
        else
            printf("No flights at the moment!\n");
        
        free(response);
        response = NULL;
        return 1;
    }
    
    free(response);
    response = NULL;
    return 0;
}

int logout(int *validLogin, int pid)
{
    pOutbound request = malloc(sizeof(Outbound));
    pInbound response;
    
    request->len = 0;
    request->pid = pid;
    request->userLevel = 2;
    request->jobRef = 2;
    response = startCommunication(request,pid);
    
    free(request);
    request = NULL;
    
    if(response->valid == 1)
    {
        *validLogin = 0;
        free(response);
        response = NULL;
        return 1;
    }
    free(response);
    response = NULL;
    return 0;
}