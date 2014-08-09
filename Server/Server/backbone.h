/*
  backbone.h
  Server

  Created by Mário Jorge Gabriel da Silva on 16/06/13.
  Copyright (c) 2013 Mário Jorge Gabriel da Silva. All rights reserved.
*/

#ifndef __Server__backbone__
#define __Server__backbone__

#include "varConf.h"
#include "ioBound.h"
#include "backbone.h"

typedef struct sBackbone Backbone, *pBackbone;
typedef struct sUsers Users, *pUsers;
typedef struct sAdmin Admin, *pAdmin;
typedef struct sCities Cities, *pCity;
typedef struct sFlights Flights, *pFlight;
typedef struct sLogger Logger, *pLogger;


struct sBackbone
{
    int date; /* System Date - Fake int */
    int nUsers; /* Number of Users */
    int nFlights; /* Number of Flights */
    int nCities; /* Number of Cities */
    int cUsers; /* Users Online */
    int indexCity; /* To control new cities */
    pUsers fUser; /* First User (Pointer) */
    pUsers lUser; /* Last User (Pointer) */
    pAdmin admin; /* Admin (Pointer) */
    pLogger logger; /* Logger (Pointer) */
    pFlight fFlight; /* First Flight (Pointer) */
    pFlight lFlight; /* Last Flight (Pointer) */
    pCity fCity; /* First City (Pointer) */
    pCity lCity; /* Last City (Pointer) */
};

struct sUsers
{
    int pid;
    char username[usernameLen];
    char password[passwordLen];
    pUsers nUser;
};

struct sAdmin
{
    int pid;
    char password[passwordLen];
};

struct sLogger
{
    int pid;
};

struct sCities
{
    int idCity;
    char cityName[cityLen];
    pCity nCity;
};

struct sFlights
{
    int idFlight;
    pCity departure;
    pCity arrival;
    int date;
    int nPassengers;
    int *Passports;
    pFlight nFlight;
};

#endif /* defined(__Server__backbone__) */