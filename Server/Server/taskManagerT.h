/*
  taskManagerT.h
  Server

  Created by Mário Jorge Gabriel da Silva on 16/06/13.
  Copyright (c) 2013 Mário Jorge Gabriel da Silva. All rights reserved.
*/

#ifndef Server_taskManagerT_h
#define Server_taskManagerT_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

#include "ioBound.h"
#include "backbone.h"
#include "serverManager.h"
#include "serverCrud.h"

int checkLoginT(pBackbone BackboneAux, int pid, char *username, char *password);
int checkPidTerm(pBackbone bbAux, int pid);
pUsers findUserPid(int pid, pBackbone BackboneAux);
int logoutT(int pid, pBackbone BackboneAux);
int flightList(int pid, pBackbone BackboneAux);
int searchFlight(int pid, char *args, pBackbone BackboneAux, int nArgs, unsigned long len);
int changePassword(int pid, char *args, pBackbone BackboneAux, int nArgs, unsigned long len);
int reservFlight(int pid, char *args, pBackbone BackboneAux, int nArgs, unsigned long len);
int cancelreservFlight(int pid, char *args, pBackbone BackboneAux, int nArgs, unsigned long len);
int argsSep(char *temp[], char *args, int nArgs);

#endif /* defined(__Server__taskManagerT__) */
