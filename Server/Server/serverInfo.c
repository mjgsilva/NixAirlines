/*
 serverInfo.c
 Server
 
 Created by Mário Jorge Gabriel da Silva on 16/06/13.
 Copyright (c) 2013 Mário Jorge Gabriel da Silva. All rights reserved.
 */

#include "serverInfo.h"

int loadDayVar(pBackbone bbAux)
{
    FILE *f;
    int day=0;
    
    f = fopen(confDay,"r");
    
    if(f==NULL)
    {
        printf("Day conf file is missing!\n");
        return 0;
    }
    
    fscanf(f,"%d",&day);
    
    if(day < 1)
        return 0;
    else
        bbAux->date = day;
    
    return 1;
    
}

pUsers loadTerminalUsers(pBackbone bbAux)
{
    FILE *f;
    FILE *g;
    int i, nUsers;
    unsigned long len;
    char *user, *pw;
    char pwadm[20];
    pUsers userAux = NULL;
    pAdmin admAux = NULL;
    pLogger lAux = NULL;
    
    f = fopen(confClients,"rb");
    g = fopen(confAdminPassword,"r");
    if(f==NULL || g==NULL)
    {
        printf("File(s) missing: clients.bin / adminpassword.txt \n");
        return NULL;
    } else {
        
        fscanf(g,"%s",pwadm);
        admAux = malloc(sizeof(Admin));
        lAux = malloc(sizeof(Logger));
        
        strcpy(admAux->password, pwadm);
        admAux->pid = 0;
        lAux->pid = 0;
        bbAux->admin = admAux;
        bbAux->logger = lAux;
        
        fread(&nUsers,sizeof(int),1,f);
        
        for(i=0; i<nUsers; i++)
        {
            fread(&len,sizeof(unsigned long),1,f);
            user = malloc(len);
            fread(user,sizeof(char),len,f);
            fread(&len,sizeof(unsigned long),1,f);
            pw = malloc(len);
            fread(pw,sizeof(char),len,f);
            userAux = addUser(bbAux,userAux,user,pw);
        }
        fclose(f);
    }
    return userAux;
}

pBackbone readMainFile(FILE *f)
{
    int cities = 0;
    int flights = 0;
    int indexCity = 0;
    int i, j, id, date, citydid, cityaid, npassengers, temppass;
    unsigned long len;
    char *temp;
    pCity cityAux = NULL;
    pCity cdepartureAux = NULL;
    pCity carrivalAux = NULL;
    pFlight flightAux = NULL;
    
    pBackbone BackBinfo = malloc(sizeof(Backbone));
    
    BackBinfo->date = 0;
    BackBinfo->nUsers = 0;
    BackBinfo->cUsers = 0;
    
    fread(&cities,sizeof(int),1,f);
    fread(&flights,sizeof(int),1,f);
    for(i=0;i<cities;i++)
    {
        fread(&id,sizeof(int),1,f);
        fread(&len,sizeof(unsigned long),1,f);
        temp = malloc(len);
        fread(temp,sizeof(char),len,f);
        cityAux = addCity(BackBinfo,cityAux,id,temp);
    }
    for(i=0;i<flights;i++)
    {
        fread(&id,sizeof(int),1,f);
        fread(&date,sizeof(int),1,f);
        fread(&citydid,sizeof(int),1,f);
        cdepartureAux = findCity(citydid,BackBinfo->fCity);
        fread(&cityaid,sizeof(int),1,f);
        carrivalAux = findCity(cityaid,BackBinfo->fCity);
        flightAux = addFlight(BackBinfo, flightAux, id, cdepartureAux, carrivalAux, date);
        fread(&npassengers,sizeof(int),1,f);
        flightAux->nPassengers = npassengers;
        if(npassengers != 0)
        {
            flightAux->Passports = malloc(sizeof(int)*npassengers);
            for(j=0;j<npassengers;j++)
            {
                fread(&temppass,sizeof(int),1,f);
                flightAux->Passports[j] = temppass;
            }
        }
    }
    fread(&indexCity,sizeof(int),1,f);
    BackBinfo->indexCity = indexCity;
    return BackBinfo;
}

pBackbone loadBackBone()
{
    FILE *f = NULL;
    pBackbone BackB = NULL;
    
    f=fopen(confFlights,"rb");
    
    if(f==NULL)
    {
        printf("Flights conf file is missing!\n");
        return NULL;
    }
    BackB = readMainFile(f);
    fclose(f);
    return BackB;
}

