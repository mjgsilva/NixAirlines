/*
  taskManagerA.c
  Server

  Created by Mário Jorge Gabriel da Silva on 16/06/13.
  Copyright (c) 2013 Mário Jorge Gabriel da Silva. All rights reserved.
*/

#include "taskManagerA.h"

int checkLoginA(pBackbone BackboneAux, int pid, char *password)
{
    pAdmin aAux;
    aAux = BackboneAux->admin;
    char msg[150];

    if((strcmp(aAux->password,password)) == 0)
    {
        if(aAux->pid == 0)
        {
            aAux->pid = pid;
            sprintf(msg,"%d | Admin logged in. Pid : %d.",BackboneAux->date,pid);
            writeLog(msg,BackboneAux->logger->pid);
            return 1;
        } else
            if(aAux->pid == pid)
                return 1;
    }
    sprintf(msg,"%d | Admin invalid log in attempt. Pid: %d.",BackboneAux->date,pid);
    writeLog(msg,BackboneAux->logger->pid);
    return 0;
}

void sendSignalToTermByPid(int pid)
{
    kill(pid, SIGUSR1);

}

void sendSignalToTerm(pBackbone BackboneAux)
{
    pUsers uAux = BackboneAux->fUser;
    
    while(uAux != NULL)
    {
        if(uAux->pid > 0)
        {
            kill(uAux->pid, SIGUSR1);
            uAux->pid = -1;
            BackboneAux->cUsers = BackboneAux->cUsers-1;
        }
        uAux = uAux->nUser;
    }
}

void sendSignalToAdmin(pBackbone BackboneAux)
{
    pAdmin adAux = BackboneAux->admin;
    
    if(adAux->pid != 0)
        kill(adAux->pid, SIGUSR1);
}

void sendSignalToLog(pBackbone BackboneAux)
{
    pLogger logAux = BackboneAux->logger;
    
    if(logAux->pid != 0)
        kill(logAux->pid, SIGUSR2);
}

void saveUsers(pBackbone bbAux)
{
    FILE *f = NULL;
    pUsers uAux = bbAux->fUser;
    int nusers;
    int i=0,j;
    unsigned long userNameLen, userPasswordLen;
    nusers = bbAux->nUsers;
    char *username[nusers];
    char *password[nusers];

    f = fopen(confClients,"wb");
    if(f != NULL)
        
    
    while(uAux != NULL)
    {
        username[i] = uAux->username;
        password[i] = uAux->password;
        i++;
        uAux = uAux->nUser;
    }
    
    
    fwrite(&nusers,sizeof(int),1,f);
    
    for(j=0; j<nusers; j++)
    {
        userNameLen=strlen(username[j])+1;
        fwrite(&userNameLen,sizeof(unsigned long),1,f);
        fwrite(username[j], sizeof(char), userNameLen, f);
        userPasswordLen=strlen(password[j])+1;
        fwrite(&userPasswordLen,sizeof(unsigned long),1,f);
        fwrite(password[j], sizeof(char), userPasswordLen, f);
    }
    fclose(f);
}

void saveFlights(pBackbone bbAux)
{
    FILE *f = NULL;
    int i, j;
    unsigned long cityNameLen;
    int k=0, l=0, p;
    int r;
    int cityIdAux;
    int flightIdAux;
    int npAux;
    int cpAux;
    pCity cityAux = bbAux->fCity;
    pFlight fAux = bbAux->fFlight;
    f = fopen(confFlights,"wb");
    
    if(f != NULL)
    {
        int totalFlights = bbAux->nFlights;
        int totalCities = bbAux->nCities;
        int cityId[totalCities];
        char *name[totalCities];
        int flightId[totalFlights];
        int day[totalFlights];
        int departureCityId[totalFlights];
        int arrivalCityId[totalFlights];
        int nPassangers[totalFlights];
        int passports[totalFlights][5];
        int index = bbAux->indexCity;
        
        while(cityAux != NULL)
        {
            cityId[k] = cityAux->idCity;
            name[k] = cityAux->cityName;
            cityAux = cityAux->nCity;
            k++;
        } 
    
        while(fAux != NULL)
        {
            flightId[l] = fAux->idFlight;
            day[l] = fAux->date;
            departureCityId[l] = fAux->departure->idCity;
            arrivalCityId[l]= fAux->arrival->idCity;
            nPassangers[l]= fAux->nPassengers;
            for(p=0;p<nPassangers[l];p++)
                passports[l][p]= fAux->Passports[p];
            fAux = fAux->nFlight; 
            l++;
        } 
    
        fwrite(&totalCities,sizeof(int),1,f);
        fwrite(&totalFlights,sizeof(int),1,f);
        for(i=0; i<totalCities; i++)
        {
            cityIdAux = cityId[i];
            fwrite(&cityIdAux,sizeof(int),1,f);
            cityNameLen=strlen(name[i])+1;
            fwrite(&cityNameLen,sizeof(unsigned long),1,f);
            fwrite(name[i], sizeof(char), cityNameLen, f);
        }
        
        for(j=0; j<totalFlights; j++)
        {
            flightIdAux = flightId[j];
            fwrite(&flightIdAux,sizeof(int),1,f);
            fwrite(&day,sizeof(int),1,f);
            fwrite(&departureCityId[j],sizeof(int),1,f);
            fwrite(&arrivalCityId[j],sizeof(int),1,f);
            npAux = nPassangers[j];
            fwrite(&npAux,sizeof(int),1,f);
            for(r=0;r<npAux;r++)
            {
                cpAux = passports[j][r];
                fwrite(&cpAux,sizeof(int),1,f);
            }
        }
        fwrite(&index,sizeof(int),1,f);
    }
    fclose(f);
}

void shutdownT(pBackbone BackboneAux)
{
    sendSignalToTerm(BackboneAux);
    sendSignalToAdmin(BackboneAux);
    sendSignalToLog(BackboneAux);
    saveFlights(BackboneAux);
    saveUsers(BackboneAux);
    printf("System is shutting down!\n");
    unlink(npServer);
    exit(0);
}

void shutdownTT() /* Using SIGUSR1 to shutdown the system - BackboneGlobal is a global variable - Very bad practice but the only way it works */
{
    sendSignalToTerm(BackboneGlobal);
    sendSignalToAdmin(BackboneGlobal);
    sendSignalToLog(BackboneGlobal);
    saveFlights(BackboneGlobal);
    saveUsers(BackboneGlobal);
    unlink(npServer);
    printf("System is shutting down!\n");
    exit(0);
}

int addUserT(char *args, pBackbone BackboneAux, int nArgs, unsigned long len)
{
    pInbound in = malloc(sizeof(Inbound));
    char *temp[2];
    char msg[150];
    
    argsSep(temp,args,nArgs);
    
    if(addUserv2(BackboneAux, temp[0], temp[1]))
    {
        sprintf(msg,"%d | User %s added.",BackboneAux->date,temp[0]);
        writeLog(msg,BackboneAux->logger->pid);
        in->valid = 1;
    } else
    {
        sprintf(msg,"%d | Error: adding user.",BackboneAux->date);
        writeLog(msg,BackboneAux->logger->pid);
        in->valid = 0;
    }
    
    if(writePipe(npAdmin,in))
        return 1;
    else
        return 0;
    
}

int addFlightT(char *args, pBackbone BackboneAux, int nArgs, unsigned long len)
{
    pInbound in = malloc(sizeof(Inbound));
    char *temp[4];
    int id;
    int fDay;
    char msg[150];
    
    argsSep(temp,args,nArgs);
    
    id = atoi(temp[0]);
    fDay = atoi(temp[3]);
    
    if(addFlightv2(BackboneAux, id, temp[1], temp[2], fDay))
    {
        sprintf(msg,"%d | Flight (%d) with Departure in %s and Arrival in %s. Day: %d.",BackboneAux->date,id,temp[1],temp[2],fDay);
        writeLog(msg,BackboneAux->logger->pid);
        in->valid = 1;
    } else {
        sprintf(msg,"%d | Error: adding flight with Departure in %s and Arrival in %s.",BackboneAux->date, temp[1], temp[2]);
        writeLog(msg,BackboneAux->logger->pid);
        in->valid = 0;
    }
    if(writePipe(npAdmin,in))
        return 1;
    else
        return 0;
    
}

int logoutAT(int pid, pBackbone BackboneAux)
{
    pAdmin adAux = BackboneAux->admin;
    pInbound in = malloc(sizeof(Inbound));
    char msg[150];
    
    adAux->pid = 0;
    
    sprintf(msg,"%d | Admin logged out.",BackboneAux->date);
    writeLog(msg,BackboneAux->logger->pid);
    in->valid = 1;
    if(writePipe(npAdmin,in))
        return 1;
    else
        return 0;
}

int getDate(pBackbone BackboneAux)
{
    pInbound in = malloc(sizeof(Inbound));
    char msg[150];
    
    if(BackboneAux->date > 0)
    {
        in->valid = BackboneAux->date;
        sprintf(msg,"%d | Admin requesting system date.",BackboneAux->date);
        writeLog(msg,BackboneAux->logger->pid);
    }
    else
    {
        in->valid = 0;
        sprintf(msg,"%d | Error occured: system date request.",BackboneAux->date);
        writeLog(msg,BackboneAux->logger->pid);
    }
    if(writePipe(npAdmin,in))
        return 1;
    else
        return 0;
}

int addCityT(char *args, pBackbone BackboneAux, int nArgs, unsigned long len)
{
    pInbound in = malloc(sizeof(Inbound));
    char *temp[1];
    char msg[150];
    
    argsSep(temp,args,nArgs);
    
    if(addCityv2(BackboneAux, temp[0]))
    {
        in->valid = 1;
        sprintf(msg,"%d | City: %s added",BackboneAux->date,temp[0]);
        writeLog(msg,BackboneAux->logger->pid);
    } else
    {
        in->valid = 0;
        sprintf(msg,"%d | Error: adding city: %s.",BackboneAux->date,temp[0]);
        writeLog(msg,BackboneAux->logger->pid);
    }
    if(writePipe(npAdmin,in))
        return 1;
    else
        return 0;
    
}

int getPast(pBackbone BackboneAux)
{
    pInboundExtended inE = malloc(sizeof(InboundExtended));
    char *hist = NULL;
    long buffer;
    size_t temp;
    unsigned long len;
    FILE *f;
    char msg[150];
    
    f = fopen(confRegistry,"r");
    
    if(f != NULL) {
        if(fseek(f, 0L, SEEK_END) == 0)
        {
            buffer = ftell(f);
            hist = malloc(sizeof(char) * (buffer+1));
            
            fseek(f, 0L, SEEK_SET);
            temp = fread(hist, sizeof(char),buffer,f);
            hist[++temp] = '\0';
   
            len = buffer;
            
            inE->valid = 1;
            inE->len = len;
            inE->report = hist;
            
        } else {
            inE->valid = 1;
            inE->len = 0;
        }
        fclose(f);
    } else {
        inE->valid = 1;
        inE->len = 0;
    }
    
    sprintf(msg,"%d | Admin requesting registry.",BackboneAux->date);
    writeLog(msg,BackboneAux->logger->pid);
    
    if(writePipeExtended(npAdmin,inE))
        return 1;
    else
        return 0;
}

int getInfoT(pBackbone BackboneAux)
{
    pInboundExtended inE = malloc(sizeof(InboundExtended));
    char info[200];
    char msg[150];
    unsigned long len;
    
    sprintf(info,"Users online: %d (total: %d)\nCities: %d | Flights: %d",BackboneAux->cUsers,BackboneAux->nUsers,BackboneAux->nCities,BackboneAux->nFlights);

    len = strlen(info)+1;
    
    sprintf(msg,"%d | Admin requesting system info.",BackboneAux->date);
    writeLog(msg,BackboneAux->logger->pid);
    
    inE->valid = 1;
    inE->len = len;
    inE->report = info;
    
    if(writePipeExtended(npAdmin,inE))
        return 1;
    else
        return 0;
}

int getCitiesT(pBackbone BackboneAux)
{
    int i=0;
    pCity cAux = BackboneAux->fCity;
    pInboundExtended inE = malloc(sizeof(InboundExtended));
    int sLen = BackboneAux->nCities * 40;
    char cities[sLen];
    char tempC[40];
    unsigned long len;
    char msg[150];
    
    if(BackboneAux->nCities > 0)
    {
        while(cAux != NULL)
        {
            sprintf(tempC,"Id: %d / %s!",cAux->idCity,cAux->cityName);
            if(i==0)
                strcpy(cities,tempC);
            else
                strcat(cities,tempC);
            i++;
            cAux=cAux->nCity;
        }
        len = strlen(cities)+1;
        inE->report = cities;
    } else {
        len = 0;
    }
    sprintf(msg,"%d | Admin asking for the Cities's List.",BackboneAux->date);
    writeLog(msg,BackboneAux->logger->pid);
    
    inE->valid = 1;
    inE->len = len;
    
    if(writePipeExtended(npAdmin,inE))
        return 1;
    else
        return 0;
}

int cancelFlightT(char *args, pBackbone BackboneAux, int nArgs, unsigned long len)
{
    pInbound in = malloc(sizeof(Inbound));
    char *temp[1];
    int fid;
    char msg[150];
    
    argsSep(temp,args,nArgs);
    
    fid = atoi(temp[0]);
    
    if(delFlight(BackboneAux, fid))
    {
        sprintf(msg,"%d | Flight n: %d removed.",BackboneAux->date, fid);
        writeLog(msg,BackboneAux->logger->pid);
        in->valid = 1;
    } else {
        sprintf(msg,"%d | Error: removing flight n: %d. Invalid flight id or Check if there is a passport on the manifest.",BackboneAux->date,fid);
        writeLog(msg,BackboneAux->logger->pid);
        in->valid = 0;
    }
    
    if(writePipe(npAdmin,in))
        return 1;
    else
        return 0;
    
}

int changeDateT(char *args, pBackbone BackboneAux, int nArgs, unsigned long len)
{
    pInbound in = malloc(sizeof(Inbound));
    char *temp[1];
    int day;
    char msg[150];
    
    argsSep(temp,args,nArgs);
    
    day = atoi(temp[0]);
    
    if(changeDate(BackboneAux,day))
    {
        sprintf(msg,"%d | System date updated.",BackboneAux->date);
        writeLog(msg,BackboneAux->logger->pid);
        in->valid = 1;
    } else {
        sprintf(msg,"%d | Error: updating system date.",BackboneAux->date);
        writeLog(msg,BackboneAux->logger->pid);
        in->valid = 0;
    }
    if(writePipe(npAdmin,in))
        return 1;
    else
        return 0;
    
}

int delCityT(char *args, pBackbone BackboneAux, int nArgs, unsigned long len)
{
    pInbound in = malloc(sizeof(Inbound));
    char *temp[1];
    char msg[150];
    
    argsSep(temp,args,nArgs);
    
    if(delCity(BackboneAux, temp[0]))
    {
        sprintf(msg,"%d | City : %s removed.",BackboneAux->date,temp[0]);
        writeLog(msg,BackboneAux->logger->pid);
        in->valid = 1;
    } else {
        sprintf(msg,"%d | Error: removing city. Invalid city?",BackboneAux->date);
        writeLog(msg,BackboneAux->logger->pid);
        in->valid = 0;
    }
    if(writePipe(npAdmin,in))
        return 1;
    else
        return 0;
    
}
