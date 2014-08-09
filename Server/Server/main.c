/*
  main.c
  Server

  Created by Mário Jorge Gabriel da Silva on 16/06/13.
  Copyright (c) 2013 Mário Jorge Gabriel da Silva. All rights reserved.
*/

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
#include "serverManager.h"
#include "serverInfo.h"
#include "serverCrud.h"
#include "switchTaskA.h"
#include "switchTaskT.h"
#include "taskManagerA.h"
#include "taskManagerT.h"

int main()
{
    int pid;
    pid = fork();
    
    if (pid < 0)
        return 0;
    else
        if(pid !=0)
            exit(0);
    else 
        serversUp();
}

