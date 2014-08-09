/*
  taskManagerA.h
  Server

  Created by Mário Jorge Gabriel da Silva on 16/06/13.
  Copyright (c) 2013 Mário Jorge Gabriel da Silva. All rights reserved.
*/

#ifndef Server_taskManagerA_h
#define Server_taskManagerA_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

#include "backbone.h"
#include "global.h"
#include "ioBound.h"
#include "communication.h"
#include "serverCrud.h"
#include "serverManager.h"
#include "taskManagerT.h"

int checkLoginA(pBackbone BackboneAux, int pid, char *password);
int addCityT(char *args, pBackbone BackboneAux, int nArgs, unsigned long len);
int delCityT(char *args, pBackbone BackboneAux, int nArgs, unsigned long len);
int addUserT(char *args, pBackbone BackboneAux, int nArgs, unsigned long len);
int getDate(pBackbone BackboneAux);
int addFlightT(char *args, pBackbone BackboneAux, int nArgs, unsigned long len);
int changeDateT(char *args, pBackbone BackboneAux, int nArgs, unsigned long len);
int cancelFlightT(char *args, pBackbone BackboneAux, int nArgs, unsigned long len);
int getInfoT(pBackbone BackboneAux);
int getCitiesT(pBackbone BackboneAux);
int getPast(pBackbone BackboneAux);
void shutdownT(pBackbone BackboneAux);
void shutdownTT();
void sendSignalToAdmin(pBackbone BackboneAux);
void sendSignalToTerm(pBackbone BackboneAux);
void sendSignalToTermByPid(int pid);
void sendSignalToLog(pBackbone BackboneAux);
int logoutAT(int pid, pBackbone BackboneAux);
void saveFlights(pBackbone bbAux);
void saveUsers(pBackbone bbAux);

#endif /* defined(__Server__taskManagerA__) */
