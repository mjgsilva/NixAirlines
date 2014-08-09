/*
 communication.h
 Terminal
 
 Created by Mário Jorge Gabriel da Silva on 18/06/13.
 Copyright (c) 2013 Mário Jorge Gabriel da Silva. All rights reserved.
 */

#ifndef __Server__communication__
#define __Server__communication__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "ioBound.h"
#include "varConf.h"

pInbound startCommunication(pOutbound req, int pid);
pInboundExtended startCommunicationExtended(pOutbound req, int pid);

#endif /* defined(__Server__communication__) */
