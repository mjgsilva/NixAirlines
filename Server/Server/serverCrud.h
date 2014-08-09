/*
  serverCrud.h
  Server

  Created by Mário Jorge Gabriel da Silva on 17/06/13.
  Copyright (c) 2013 Mário Jorge Gabriel da Silva. All rights reserved.
*/

#ifndef Server_serverCrud_h
#define Server_serverCrud_h

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "backbone.h"
#include "varConf.h"
#include "ioBound.h"
#include "communication.h"
#include "serverManager.h"
#include "serverInfo.h"

int isUsersListEmpty(pUsers uAux);
int isCityListEmpty(pCity cityList);
int isFlightListEmpty(pFlight flightList);
pCity addCity(pBackbone bbAux, pCity cityList, int idCity, char *cityName);
int delCity(pBackbone bbAux, char *cityName);
int addCityv2(pBackbone bbAux, char *cityName);
int checkCityFlight(pBackbone bbAux, char *cityName);
pCity findCity(int idCity, pCity fCity);
pCity findCityByName(char *cityName, pCity fCity);
int findFlight(int idFlight, pBackbone bbAux);
pFlight addFlight(pBackbone bbAux, pFlight flightList, int idFlight, pCity cDeparture, pCity cArrival, int date);
int addFlightv2(pBackbone bbAux,int idFlight, char *departure, char *arrival, int date);
pUsers addUser(pBackbone bbAux, pUsers usersList, char *username, char *password);
int addUserv2(pBackbone bbAux, char *username, char *password);
int findUser(pBackbone bbAux, char *username);
int changeDate(pBackbone bbAux, int day);
int delFlight(pBackbone bbAux, int fid);
pFlight checkidFlight(pBackbone bbAux, int fid);
int writeHistSepLine(int day);
int writeHist(int fid, char *cDeparture, char *cArrival, int nPassengers, int day);
void loggersUp(pBackbone bbAux, int pid);
void loggersDown(pBackbone bbAux);
void writeLog(char *msg, int isLogged);

#endif /* defined(__Server__serverCrud__) */

