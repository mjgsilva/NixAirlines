/*
  serverInfo.h
  Server

  Created by Mário Jorge Gabriel da Silva on 17/06/13.
  Copyright (c) 2013 Mário Jorge Gabriel da Silva. All rights reserved.
*/

#ifndef Server_serverInfo_h
#define Server_serverInfo_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "backbone.h"
#include "varConf.h"
#include "serverCrud.h"

pBackbone loadBackBone();
pBackbone readMainFile(FILE *f);
pUsers loadTerminalUsers(pBackbone bbAux);
int loadDayVar(pBackbone bbAux);

#endif /* defined(__Server__serverInfo__) */
