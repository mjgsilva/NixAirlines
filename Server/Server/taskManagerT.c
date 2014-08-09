/*
  taskManagerT.c
  Server

  Created by Mário Jorge Gabriel da Silva on 16/06/13.
  Copyright (c) 2013 Mário Jorge Gabriel da Silva. All rights reserved.
*/

#include "taskManagerT.h"

int checkLoginT(pBackbone BackboneAux, int pid, char *username, char *password)
{
    pUsers uAux = BackboneAux->fUser;
    pInbound in = malloc(sizeof(Inbound));
    char npName[maxnp];
    char msg[150];
    sprintf(npName,"%d",pid);
        
    while(uAux != NULL)
    {
       if(strcmp(uAux->username,username) == 0)
           if(strcmp(uAux->password, password) == 0)
           {
               uAux->pid = pid;
               BackboneAux->cUsers = BackboneAux->cUsers + 1;
               in->valid = 1;
               writePipe(npName,in);
               sprintf(msg,"%d | User %s w/ pid %d logged in.",BackboneAux->date,username,pid);
               writeLog(msg,BackboneAux->logger->pid);
               return 1;
           }
        uAux = uAux->nUser;
    }
    sprintf(msg,"%d | Login attempt failed. Pid %d.",BackboneAux->date,pid);
    writeLog(msg,BackboneAux->logger->pid);
    in->valid = 0;
    writePipe(npName,in);
    return 0;
}

int checkPidTerm(pBackbone bbAux, int pid)
{
    pUsers uAux = bbAux->fUser;
    
    while (uAux != NULL)
    {
        if(uAux->pid == pid)
            return 1;
        uAux=uAux->nUser;
    }
    return 0;
}

pUsers findUserPid(int pid, pBackbone BackboneAux)
{
    pUsers uAux;
    uAux = BackboneAux->fUser;

     while(uAux != NULL)
    {
        if(uAux->pid == pid)
        {
            return uAux;
        }
        uAux = uAux->nUser;
    }
    return NULL; 
}

int logoutT(int pid, pBackbone BackboneAux)
{
    pUsers uAux;
    pInbound in = malloc(sizeof(Inbound)); 
    char npName[maxnp];
    sprintf(npName,"%d",pid);
    char msg[150];

    uAux = findUserPid(pid, BackboneAux);
    if(uAux == NULL)
        return 0;
    uAux->pid = -1;
    BackboneAux->cUsers = BackboneAux->cUsers - 1;
    
    sprintf(msg,"%d | User %s w/ pid %d Logged out.",BackboneAux->date,uAux->username,pid);
    writeLog(msg,BackboneAux->logger->pid);
    in->valid = 1;
    
    if(writePipe(npName,in))
        return 1;
    else
        return 0;
}

int argsSep(char *temp[], char *args, int nArgs)
{
    int i;
    char *temp2;
    
    temp2 = strtok(args, ":");
    temp[0] = temp2;
    for(i=1;i<nArgs;i++)
    {
        temp2 = strtok(NULL, ":");
        temp[i] = temp2;
    }
    return 1;
    
}

int changePassword(int pid, char *args, pBackbone BackboneAux, int nArgs, unsigned long len)
{
    pUsers uAux;
    pInbound in = malloc(sizeof(Inbound));
    char npName[maxnp];
    char *temp[2];
    sprintf(npName,"%d",pid);
    char msg[150];
    
    argsSep(temp,args,nArgs);
    
    uAux = findUserPid(pid,BackboneAux);
    if(strcmp(uAux->password,temp[0]) == 0)
    {
        strcpy(uAux->password,temp[1]);
        in->valid = 1;
        sprintf(msg,"%d | User %s changed his password.",BackboneAux->date,uAux->username);
        writeLog(msg,BackboneAux->logger->pid);
    } else
        in->valid = 0;
    
    
    if(writePipe(npName,in))
        return 1;
    else
        return 0;
}

int reservFlight(int pid, char *args, pBackbone BackboneAux, int nArgs, unsigned long len)
{
    int i,j=0;
    pFlight fAux = BackboneAux->fFlight;
    pInbound in = malloc(sizeof(Inbound));
    char *tempArgs[2];
    char npName[maxnp];
    sprintf(npName,"%d",pid);
    int idvoo;
    int passport;
    char msg[150];
    
    argsSep(tempArgs,args,nArgs);
    
    idvoo = atoi(tempArgs[0]);
    passport = atoi(tempArgs[1]);
    
    while(fAux != NULL)
    {
        if(fAux->idFlight == idvoo)
        {
            if(fAux->nPassengers == 0)
            {
                fAux->Passports = malloc(sizeof(int));

            } else
                if(fAux->nPassengers < maxPassengers)
                {

                    for(i=0;i<fAux->nPassengers && j!=1;i++)
                    {
                        if(fAux->Passports[i] == passport)
                            j=1;
                    }
                    break;
                } else
                    j=1;
            break;
            }
        fAux=fAux->nFlight;
    }

    
    if(j != 1 && fAux != NULL)
    {
        if(fAux->nPassengers != 0)
        {
            fAux->Passports = realloc(fAux->Passports,sizeof(int) * (fAux->nPassengers + 1));
        }
        fAux->Passports[fAux->nPassengers] = passport;
        fAux->nPassengers = fAux->nPassengers + 1;
        in->valid = 1;
        sprintf(msg,"%d | Passenger w/ Passport %d added to the manifest of the Flight %d.",BackboneAux->date,passport,fAux->idFlight);
        writeLog(msg,BackboneAux->logger->pid);
    } else {
        in->valid = 0;
        sprintf(msg,"%d | Invalid attempt of adding a Passenger to a Flight from Terminal %d.",BackboneAux->date,pid);
        writeLog(msg,BackboneAux->logger->pid);
    }
    
    if(writePipe(npName,in))
        return 1;
    else
        return 0;
    
}

int cancelreservFlight(int pid, char *args, pBackbone BackboneAux, int nArgs, unsigned long len)
{
    int i,j=0;
    pFlight fAux = BackboneAux->fFlight;
    pInbound in = malloc(sizeof(Inbound));
    char *tempArgs[2];
    char npName[maxnp];
    sprintf(npName,"%d",pid);
    int idvoo;
    int passport;
    char msg[150];

    argsSep(tempArgs,args,nArgs);
    
    idvoo = atoi(tempArgs[0]);
    passport = atoi(tempArgs[1]);
    
    while(fAux != NULL)
    {
        if(fAux->idFlight == idvoo)
        {
                for(i=0;i<fAux->nPassengers && j!=1;i++)
                {
                    if(fAux->Passports[i] == passport)
                        j=1;
                }
                break;
        }
        fAux=fAux->nFlight;
    }
    
    if(j == 1)
    {
        if(i == 0)
        {
            if(fAux->nPassengers == 1)
                free(fAux->Passports);
            if(fAux->nPassengers > 1)
            {
                fAux->Passports[0] = fAux->Passports[fAux->nPassengers-1];
                fAux->Passports = realloc(fAux->Passports,(fAux->nPassengers - 1) * sizeof(int));
            }
        /* Todas as pos que não a primeira */
        } else {
            fAux->Passports[i-1] = fAux->Passports[fAux->nPassengers-1];
            fAux->Passports = realloc(fAux->Passports,(fAux->nPassengers - 1) * sizeof(int));
        }
        
        fAux->nPassengers = fAux->nPassengers - 1;
        in->valid = 1;
        sprintf(msg,"%d | Passenger w/ Passport %d removed from the manifest of the Flight %d.",BackboneAux->date,passport,fAux->idFlight);
        writeLog(msg,BackboneAux->logger->pid);
    } else {
     in->valid = 0;
     sprintf(msg,"%d | Terminal %d tryed to remove an unregisted Passenger (%d) from the Flight %d.",BackboneAux->date,pid,passport,fAux->idFlight);
        writeLog(msg,BackboneAux->logger->pid);
    }
    if(writePipe(npName,in))
        return 1;
    else
        return 0;
    
}

int searchFlight(int pid, char *args, pBackbone BackboneAux, int nArgs, unsigned long len)
{
    int i=0;
    pFlight fAux = BackboneAux->fFlight;
    pInboundExtended inE = malloc(sizeof(InboundExtended));
    char flight[2500];
    char tempf[2500];
    char *tempArgs[2];
    char npName[maxnp];
    sprintf(npName,"%d",pid);
    unsigned long lenf;
    char msg[150];
    
    argsSep(tempArgs,args,nArgs);
    
    while(fAux != NULL)
    {
        if(strcmp(fAux->departure->cityName,tempArgs[0]) == 0 && strcmp(fAux->arrival->cityName,tempArgs[1]) == 0)
        {
            sprintf(tempf,"Id: %d / Departure: %s / Arrival: %s",fAux->idFlight,fAux->departure->cityName,fAux->arrival->cityName);
            if(i==0)
                strcpy(flight,tempf);
            else
                strcat(flight,tempf);
            i++;
        }
        fAux=fAux->nFlight;
    }
    lenf = strlen(flight)+1;
    
    if(i != 0)
    {
        inE->valid = 1;
        inE->len = lenf;
        inE->report = flight;
        sprintf(msg,"%d | Terminal %d asking for information about a Flight between %s and %s.",BackboneAux->date,pid,tempArgs[0],tempArgs[1]);
        writeLog(msg,BackboneAux->logger->pid);
    } else {
        inE->valid = 0;
        sprintf(msg,"%d | Terminal %d asking for information about an invalid Flight.",BackboneAux->date,pid);
        writeLog(msg,BackboneAux->logger->pid);
    }
    
    if(writePipeExtended(npName,inE))
        return 1;
    else
        return 0;
}

int flightList(int pid, pBackbone BackboneAux)
{
    int i=0, j;
    pFlight fAux = BackboneAux->fFlight;
    pInboundExtended inE = malloc(sizeof(InboundExtended));
    int sLen = BackboneAux->nFlights * 200;
    char flight[sLen];
    char tempFlight[200];
    char tempPassengers[100];
    char npName[maxnp];
    sprintf(npName,"%d",pid);
    unsigned long len;
    char msg[150];
    
    if(BackboneAux->nFlights > 0)
    {
        while(fAux != NULL)
        {
            sprintf(tempFlight,"Id: %d / Departure: %s / Arrival: %s w/ %d Passengers on Day %d!",fAux->idFlight,fAux->departure->cityName,fAux->arrival->cityName,fAux->nPassengers,fAux->date);
            
            if(fAux->nPassengers > 0)
            {
                sprintf(tempPassengers,"\tManifest:");
                strcat(tempFlight,tempPassengers);
                for(j = 0; j < fAux->nPassengers; j++)
                {
                    sprintf(tempPassengers," %d ", fAux->Passports[j]);
                    strcat(tempFlight, tempPassengers);
                    if(j < fAux->nPassengers - 1)
                    {
                        sprintf(tempPassengers, "/");
                        strcat(tempFlight, tempPassengers);
                    }
                }
                strcat(tempFlight,"\n");
            }
            
            if(i==0)
                strcpy(flight,tempFlight);
            else
                strcat(flight,tempFlight);
            i++;
            fAux=fAux->nFlight;
        }
        len = strlen(flight)+1;
        inE->report = flight;
    } else {
        len = 0;
    }

    sprintf(msg,"%d | Terminal %d asking for the Flights List.",BackboneAux->date,pid);
    writeLog(msg,BackboneAux->logger->pid);
    
    inE->valid = 1;
    inE->len = len;
    
    if(writePipeExtended(npName,inE))
        return 1;
    else
        return 0;
}


