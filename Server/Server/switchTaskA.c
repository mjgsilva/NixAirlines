/*
  switchTaskA.c
  Server

  Created by Mário Jorge Gabriel da Silva on 16/06/13.
  Copyright (c) 2013 Mário Jorge Gabriel da Silva. All rights reserved.
*/

#include "switchTaskA.h"

int findTaskA(int jobRef, int nArgs, char *args, int pid, unsigned long len, pBackbone BackboneAux)
{
      switch(jobRef)
    {
        case 2:
            shutdownT(BackboneAux);
            break;
        case 3:
            if(addCityT(args,BackboneAux,nArgs,len))
                return 1;
            break;
        case 4:
            if(delCityT(args,BackboneAux,nArgs,len))
                return 1;
            break;
        case 5:
            if(getPast(BackboneAux))
                return 1;
            break;
        case 6:
            if(addFlightT(args,BackboneAux,nArgs,len))
                return 1;
            break;
        case 7:
            if(cancelFlightT(args,BackboneAux,nArgs,len))
                return 1;
            break;
        case 8:
            if(changeDateT(args,BackboneAux,nArgs,len))
                return 1;
            break;
        case 9:
            if(getDate(BackboneAux))
                return 1;
            break;
        case 10:
            if(getInfoT(BackboneAux))
                return 1;
            break;
        case 11:
            if(addUserT(args,BackboneAux,nArgs,len))
                return 1;
            break;
        case 12:
            logoutAT(pid, BackboneAux);
            break;
        case 13:
            if(getCitiesT(BackboneAux))
                return 1;
            break;
    }
    return 0;
}