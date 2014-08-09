/*
  switchTastA.h
  Server

  Created by Mário Jorge Gabriel da Silva on 16/06/13.
  Copyright (c) 2013 Mário Jorge Gabriel da Silva. All rights reserved.
*/

#ifndef Server_switchTaskA_h
#define Server_switchTaskA_h

#include <stdio.h>

#include "backbone.h"
#include "taskManagerA.h"
#include "serverCrud.h"

int findTaskA(int jobRef, int nArgs, char *args, int pid, unsigned long len, pBackbone BackboneAux);

#endif /* defined(__Server__switchTaskA__) */
