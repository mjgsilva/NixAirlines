/*
  switchTaskT.c
  Server

  Created by Mário Jorge Gabriel da Silva on 16/06/13.
  Copyright (c) 2013 Mário Jorge Gabriel da Silva. All rights reserved.
*/

#include "taskManagerT.h"

int findTaskT(int jobRef, int nArgs, char *args, int pid, unsigned long len, pBackbone BackboneAux)
{
     switch(jobRef)
    {
        case 2:
            if(logoutT(pid,BackboneAux))
               return 1;
            break;
        case 3:
            if(changePassword(pid,args,BackboneAux,nArgs,len))
                return 1;
            break;
        case 4:
            if(flightList(pid,BackboneAux))
                return 1;
            break;
        case 5:
            if(searchFlight(pid,args,BackboneAux,nArgs,len))
                return 1;
            break;
        case 6:
            if(reservFlight(pid,args,BackboneAux,nArgs,len))
                return 1;
            break;
        case 7:
            if(cancelreservFlight(pid,args,BackboneAux,nArgs,len))
                return 1;
            break;
    }
    return 0;
}