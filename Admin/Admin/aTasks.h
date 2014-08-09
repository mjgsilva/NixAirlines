//
//  aTasks.h
//  Admin
//
//  Created by Mário Silva on 07/08/14.
//  Copyright (c) 2014 Mário Jorge Gabriel da Silva. All rights reserved.
//

#ifndef __Server__aTasks__
#define __Server__aTasks__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

#include "varConf.h"
#include "ioBound.h"
#include "communication.h"

int shutdown(int pid, char *password);
int delCity(char *extendedArgs, int pid, char *password);
int addCity(char *extendedArgs, int pid, char *password);
int delFlight(char *extendedArgs, int pid, char *password);
int changeDate(char *extendedArgs, int pid, char *password);
int getInfo(int pid, char *password);
int getPast(int pid, char *password);
int getCities(int pid, char *password);
int addUser(char *extendedArgs, int pid, char *password);
int addFlight(char *extendedArgs, int pid, char *password);
int logout(int *validLogin, int pid, char *password);
int getDate(int pid, char *password);
void shutAll();

#endif /* defined(__Server__aTasks__) */
