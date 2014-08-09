/*
  serverManager.h
  Server

  Created by Mário Jorge Gabriel da Silva on 17/06/13.
  Copyright (c) 2013 Mário Jorge Gabriel da Silva. All rights reserved.
*/

#ifndef Server_serverManager_h
#define Server_serverManager_h

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
#include "varConf.h"
#include "global.h"
#include "ioBound.h"
#include "communication.h"
#include "taskManagerA.h"
#include "taskManagerT.h"
#include "switchTaskA.h"
#include "switchTaskT.h"
#include "serverCrud.h"
#include "serverInfo.h"

int createPipe();
int serversUp();

#endif /* defined(__Server__serverManager__) */
