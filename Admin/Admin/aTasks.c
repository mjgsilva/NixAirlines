//
//  aTasks.c
//  Admin
//
//  Created by Mário Silva on 07/08/14.
//  Copyright (c) 2014 Mário Jorge Gabriel da Silva. All rights reserved.
//

#include "aTasks.h"

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

void shutAll()
{
    printf("Central is shutting down!\n");
    unlink(npAdmin);
    exit(0);
}

int getDate(int pid, char *password)
{
    pOutbound request = malloc(sizeof(Outbound));
    pInbound response;
    
    request->len = 0;
    request->pid = pid;
    request->userLevel = 1;
    request->jobRef = 9;
    strcpy(request->pw,password);
    
    response = startCommunication(request,pid);
    
    free(request);
    request = NULL;
    
    if(response->valid > 0)
    {
        printf("Date: %d\n", response->valid);
        return 1;
    } else
        return 0;
}

int shutdown(int pid, char *password)
{
    pOutbound request = malloc(sizeof(Outbound));
    pInbound response;
    
    request->len = 0;
    request->pid = pid;
    request->userLevel = 1;
    request->jobRef = 2;
    strcpy(request->pw,password);
    
    response = startCommunication(request,pid);
    free(request);
    request = NULL;
    return 0;
}

int delCity(char *extendedArgs, int pid, char *password)
{
    pOutbound request = malloc(sizeof(Outbound));
    pInbound response;
    unsigned long len;
    len = strlen(extendedArgs)+1;
    
    request->len = len;
    request->pid = pid;
    request->userLevel = 1;
    request->jobRef = 4;
    request->nArgs = 1;
    strcpy(request->pw,password);
    request->args = extendedArgs;
    
    response = startCommunication(request,pid);
    
    free(request);
    request = NULL;
    
    if(response->valid == 1)
    {
        printf("City removed: %s.\n", request->args);
        return 1;
    } else {
        printf("Error: removing city. City does not exists or Check if there is some flight using this city as departure or arrival.\n");
    }
    return 0;
}

int addCity(char *extendedArgs, int pid, char *password)
{
    pOutbound request = malloc(sizeof(Outbound));
    pInbound response;
    unsigned long len;
    len = strlen(extendedArgs)+1;
    
    request->len = len;
    request->pid = pid;
    request->userLevel = 1;
    request->jobRef = 3;
    request->nArgs = 1;
    strcpy(request->pw,password);
    request->args = extendedArgs;
    
    response = startCommunication(request,pid);
    
    free(request);
    request = NULL;
    
    if(response->valid == 1)
    {
        printf("City added: %s.\n", request->args);
        return 1;
    } else {
        printf("Error: adding city. Check if the city you specified is already in the system.\n");
    }
    return 0;
}

int delFlight(char *extendedArgs, int pid, char *password)
{
    pOutbound request = malloc(sizeof(Outbound));
    pInbound response;
    unsigned long len;
    len = strlen(extendedArgs)+1;
    
    request->len = len;
    request->pid = pid;
    request->userLevel = 1;
    request->jobRef = 7;
    request->nArgs = 1;
    strcpy(request->pw,password);
    request->args = extendedArgs;
    
    response = startCommunication(request,pid);
    
    free(request);
    request = NULL;
    
    if(response->valid == 1)
    {
        printf("Flight removed!\n");
        return 1;
    } else {
        printf("Error: removing flight. Check if the Flight ID you specified is valid.\n");
    }
    return 0;
}

int changeDate(char *extendedArgs, int pid, char *password)
{
    pOutbound request = malloc(sizeof(Outbound));
    pInbound response;
    unsigned long len;
    len = strlen(extendedArgs)+1;
    
    request->len = len;
    request->pid = pid;
    request->userLevel = 1;
    request->jobRef = 8;
    request->nArgs = 1;
    strcpy(request->pw,password);
    request->args = extendedArgs;
    
    response = startCommunication(request,pid);
    
    free(request);
    request = NULL;
    
    if(response->valid == 1)
    {
        printf("Date: updated!\n");
        return 1;
    } else {
        printf("Error: changing date. Invalid date or permission file problem\n");
    }
    return 0;
}

int getInfo(int pid, char *password)
{
    pOutbound request = malloc(sizeof(Outbound));
    pInboundExtended response;
    
    request->len = 0;
    request->pid = pid;
    request->userLevel = 1;
    request->jobRef = 10;
    strcpy(request->pw,password);
    
    response = startCommunicationExtended(request,pid);
    
    free(request);
    request = NULL;
    
    if(response->valid == 1)
    {
        if(response->len > 0)
        {
            responseText(response);
        }
        else
            printf("No information available!\n");
        return 1;
    }
    return 0;
}

int getPast(int pid, char *password)
{
    pOutbound request = malloc(sizeof(Outbound));
    pInboundExtended response;
    
    request->len = 0;
    request->pid = pid;
    request->userLevel = 1;
    request->jobRef = 5;
    strcpy(request->pw,password);
    
    response = startCommunicationExtended(request,pid);
    
    free(request);
    request = NULL;
    
    if(response->valid == 1)
    {
        if(response->len > 0)
            responseText(response);
        else
            printf("Histórico Vazio!\n");
        return 1;
    }
    return 0;
}

int getCities(int pid, char *password)
{
    pOutbound request = malloc(sizeof(Outbound));
    pInboundExtended response;
    
    request->len = 0;
    request->pid = pid;
    request->userLevel = 1;
    request->jobRef = 13;
    strcpy(request->pw,password);
    
    response = startCommunicationExtended(request,pid);
    
    free(request);
    request = NULL;
    
    if(response->valid == 1)
    {
        if(response->len > 0)
            responseText(response);
        else
            printf("Cities's list is empty.\n");
        return 1;
    }
    return 0;
}

int addUser(char *extendedArgs, int pid, char *password)
{
    pOutbound request = malloc(sizeof(Outbound));
    pInbound response;
    unsigned long len;
    len = strlen(extendedArgs)+1;
    
    request->len = len;
    request->pid = pid;
    request->userLevel = 1;
    request->jobRef = 11;
    request->nArgs = 2;
    strcpy(request->pw,password);
    request->args = extendedArgs;
    
    free(request);
    request = NULL;
    
    response = startCommunication(request,pid);
    
    if(response->valid == 1)
    {
        printf("User added!\n");
        return 1;
    } else {
        printf("Error: inserting user. Check if the username you specified is already in use\n");
    }
    return 0;
}

int addFlight(char *extendedArgs, int pid, char *password)
{
    pOutbound request = malloc(sizeof(Outbound));
    pInbound response;
    unsigned long len;
    len = strlen(extendedArgs)+1;
    
    request->len = len;
    request->pid = pid;
    request->userLevel = 1;
    request->jobRef = 6;
    request->nArgs = 4;
    strcpy(request->pw,password);
    request->args = extendedArgs;
    
    response = startCommunication(request,pid);
    
    free(request);
    request = NULL;
    
    if(response->valid == 1)
    {
        printf("Flight added!\n");
        return 1;
    } else {
        printf("Error: adding flight. Invalid cities, wrong ID or invalid date\n");
    }
    return 0;
}

int logout(int *validLogin, int pid, char *password)
{
    pOutbound request = malloc(sizeof(Outbound));
    pInbound response;
    
    request->len = 0;
    request->pid = pid;
    request->userLevel = 1;
    request->jobRef = 12;
    strcpy(request->pw,password);

    response = startCommunication(request,pid);
    
    free(request);
    request = NULL;
    
    if(response->valid == 1)
    {
        validLogin = 0;
        return 1;
    }
    return 0;
}