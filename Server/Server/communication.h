//
//  communication.h
//  Server
//
//  Created by Mário Silva on 07/08/14.
//  Copyright (c) 2014 Mário Jorge Gabriel da Silva. All rights reserved.
//

#ifndef __Server__communication__
#define __Server__communication__

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

#include "ioBound.h"

int writePipeExtended(char *npN, pInboundExtended out);
int writePipe(char *npN, pInbound out);


#endif /* defined(__Server__communication__) */
