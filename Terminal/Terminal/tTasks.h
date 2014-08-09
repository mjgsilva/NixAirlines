//
//  tTasks.h
//  Terminal
//
//  Created by Mário Silva on 07/08/14.
//  Copyright (c) 2014 Mário Jorge Gabriel da Silva. All rights reserved.
//

#ifndef __Server__tTasks__
#define __Server__tTasks__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

#include "varConf.h"
#include "ioBound.h"
#include "communication.h"

int cancelFlight(char *extendedArgs, int pid); /* jobRef = 7 */
int bookFlight(char *extendedArgs, int pid); /* jobRef = 6 */
int searchFlight(char *extendedArgs, int pid); /* jobRef = 5 */
int changePassword(char *extendedArgs, int pid); /* jobRef = 3 */
int flightList(int pid); /* jobRef = 4 */
int logout(int *validLogin, int pid); /* jobRef = 2 */
void shutdownTerm();

#endif /* defined(__Server__tTasks__) */
