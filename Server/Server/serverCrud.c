/*
  serverCrud.c
  Server

  Created by Mário Jorge Gabriel da Silva on 17/06/13.
  Copyright (c) 2013 Mário Jorge Gabriel da Silva. All rights reserved.
*/

#include "serverCrud.h"

int isUsersListEmpty(pUsers uAux)
{
    if(uAux == NULL)
        return 1;
    else
        return 0;
}

int isCityListEmpty(pCity cityList)
{
    if(cityList == NULL)
    {
        return 1;
    } else
    {
        return 0;
    }
}

int isFlightListEmpty(pFlight flightList)
{
    if(flightList == NULL)
        return 1;
    else
        return 0;
}

void loggersUp(pBackbone bbAux, int pid)
{
    bbAux->logger->pid = pid;
}

void loggersDown(pBackbone bbAux)
{
    bbAux->logger->pid = 0;
}

void writeLog(char *msg, int isLogged)
{
    if(isLogged > 0)
    {
        pInboundExtended inE = malloc(sizeof(InboundExtended));
        unsigned long len;
    
        len = strlen(msg)+1;
    
        inE->valid = 1;
        inE->len = len;
        inE->report = msg;
        writePipeExtended(npLog,inE);
    }
}

pUsers addUser(pBackbone bbAux, pUsers usersList, char *username, char *password)
{
    pUsers userAux;
    
    userAux = malloc(sizeof(Users));
    
    if(userAux == NULL)
    {
        printf("Allocation error: New user!\n");
        return usersList;
    }
    
    if(isUsersListEmpty(usersList))
        bbAux->fUser = userAux;
    else
        usersList->nUser = userAux;
    
    strcpy(userAux->username,username);
    strcpy(userAux->password,password);
    userAux->nUser = NULL;
    bbAux->nUsers = bbAux->nUsers + 1;
    bbAux->lUser = userAux;
    return userAux;
}

int findUser(pBackbone bbAux, char *username)
{
    pUsers userAux;
    userAux = bbAux->fUser;
    
    while(userAux != NULL)
    {
        if(strcmp(userAux->username,username) == 0)
            return 1;
        userAux = userAux->nUser;
    }
    return 0;
}

int addUserv2(pBackbone bbAux, char *username, char *password)
{
    pUsers newUser;
    
    newUser = malloc(sizeof(Users));
    
    if(newUser == NULL)
    {
        printf("Allocation error: New user!\n");
        return 0;
    }
    
    if(isUsersListEmpty(bbAux->fUser))
    {
        bbAux->fUser = newUser;
    }
    
    if(findUser(bbAux,username))
    {
        return 0;
    }
    
    bbAux->lUser->nUser = newUser;
    bbAux->lUser = newUser;
    bbAux->nUsers = bbAux->nUsers++;
    sprintf(newUser->username,"%s",username);
    sprintf(newUser->password,"%s",password);
    newUser->nUser = NULL;
        
    return 1;
}

pCity addCity(pBackbone bbAux, pCity cityList, int idCity, char *cityName)
{
    pCity cityAux;
    
    cityAux = malloc(sizeof(Cities));
    
    if(cityAux == NULL)
    {
        printf("Allocation error: New city!\n");
        return cityList;
    }
    
    
    if(isCityListEmpty(cityList))
        bbAux->fCity = cityAux;
    else
        cityList->nCity = cityAux;
    
    cityAux->idCity = idCity;
    strcpy(cityAux->cityName,cityName);
    cityAux->nCity = NULL;
    bbAux->nCities = bbAux->nCities + 1;
    bbAux->lCity = cityAux;

    return cityAux;
}

pCity findCityv2(pBackbone bbAux, char *cityName)
{
    pCity cityAux = malloc(sizeof(Cities));
    cityAux = bbAux->fCity;
    
    while(cityAux != NULL)
    {
        if(strcmp(cityAux->cityName,cityName) == 0)
            return cityAux;
        cityAux = cityAux->nCity;
    }
    return NULL;
}

int checkCityFlight(pBackbone bbAux, char *cityName)
{
    pFlight fAux;
    fAux = bbAux->fFlight;
    
    while(fAux != NULL)
    {
        if((strcmp(fAux->departure->cityName, cityName) == 0) || (strcmp(fAux->arrival->cityName, cityName) == 0))
        return 1;
        fAux = fAux->nFlight;
    }
    return 0;
}

int addCityv2(pBackbone bbAux, char *cityName)
{
    pCity cAux;
    pCity newCity;
    
    newCity = malloc(sizeof(Cities));
    cAux = malloc(sizeof(Cities));
    
    if(newCity == NULL || cAux == NULL)
    {
        printf("Allocation error: New City!\n");
        return 0;
    }
    
    if(isCityListEmpty(bbAux->fCity))
    {
        bbAux->fCity = newCity;
    }
    
    if((cAux=findCityv2(bbAux,cityName)) != NULL)
    {
        return 0;
    }
        
    bbAux->lCity->nCity = newCity;
    bbAux->lCity = newCity;
    bbAux->nCities = bbAux->nCities + 1;
    sprintf(newCity->cityName,"%s",cityName);
    newCity->nCity = NULL;
    newCity->idCity = ++bbAux->indexCity;
    
    return 1;
}

pFlight checkidFlight(pBackbone bbAux, int fid)
{
    pFlight fAux = malloc(sizeof(Flights));
    fAux = bbAux->fFlight;
    
    while(fAux != NULL)
    {
        if(fAux->idFlight == fid)
            return fAux;
        fAux = fAux->nFlight;
    }
    free(fAux);
    return 0;
}

int delFlight(pBackbone bbAux, int fid)
{
    pFlight delFlight;
    pFlight fAux;
    pFlight fAux2;
    
    delFlight = malloc(sizeof(Flights));
    fAux = malloc(sizeof(Flights));
    fAux2 = malloc(sizeof(Flights));
    
    if(delFlight == NULL || fAux == NULL || fAux2 == NULL)
    {
        printf("Allocation error: Removing flight!\n");
        return 0;
    }
    
    fAux = bbAux->fFlight;
    fAux2 = fAux->nFlight;
    
    if(isFlightListEmpty(bbAux->fFlight))
    {
        return 0;
    }
    
    if((delFlight=checkidFlight(bbAux,fid)) == NULL)
    {
        return 0;
    } else {
        if(bbAux->nFlights == 1)
        {
            bbAux->fFlight = NULL;
            bbAux->lFlight = NULL;
            bbAux->nFlights = 0;
            free(delFlight);
        } else {
            if (bbAux->nFlights == 2)
            {
                if(fAux == delFlight) /*1 pos*/
                {
                    bbAux->fFlight = fAux2;
                    free(fAux);
                } else {
                    bbAux->lFlight = fAux;
                    free(fAux2);
                }
                bbAux->nFlights = 1;
            } else {
                if(fAux->idFlight == delFlight->idFlight)
                {
                    bbAux->fFlight = fAux2;
                    free(fAux);
                } else {
                    while(fAux != NULL)
                    {
                        if(fAux2->idFlight == delFlight->idFlight)
                        {
                            fAux->nFlight = fAux2->nFlight;
                            free(fAux2);
                            break;
                        } else {
                            if(fAux2 == NULL)
                            {
                                fAux->nFlight = NULL;
                                bbAux->lFlight = fAux;
                                free(fAux2);
                            }
                        }
                        fAux = fAux->nFlight;
                        fAux2 = fAux->nFlight;
                    }
                }
                bbAux->nFlights = bbAux->nFlights - 1;
            }
        }
    }
    return 1;
}

int writeHistSepLine(int day)
{
    FILE *f;
    f = fopen(confRegistry, "a");
    
    if(f==NULL)
    {
        printf("Error: Cannot open registry file!\n");
        return 0;
    }
    
    fprintf(f,"-------------------- Day %d --------------------\n", day);
    fclose(f);
    return 1;
}

int writeHist(int fid, char *cDeparture, char *cArrival, int nPassengers, int day)
{
    FILE *f;
    f = fopen(confRegistry, "a");
    
    if(f==NULL)
    {
        printf("Error: Registry file!\n");
        return 0;
    }
    fprintf(f,"Flight %d - Departure: %s / Arrival: %s / Date: %d / Passengers: %d \n",fid,cDeparture,cArrival,day,nPassengers);

    fclose(f);
    return 1;
}

int changeDate(pBackbone bbAux, int day)
{
    pFlight fAux = bbAux->fFlight;
    int *idFlightForHist = malloc(sizeof(int));
    int j;
    int k=0;
    FILE *f;
    
    if (day <= bbAux->date)
    {
        return 0;
    } else {
        bbAux->date = day;
    
        while(fAux != NULL)
        {
            if(fAux->date < day)
            {
                idFlightForHist = realloc(idFlightForHist,(k+1) * sizeof(int));
                idFlightForHist[k] = fAux->idFlight;
                k++;                
                if(!writeHist(fAux->idFlight, fAux->departure->cityName, fAux->arrival->cityName, fAux->nPassengers, fAux->date))
                    return 0;
            }
            fAux = fAux->nFlight;
        }
        
        if(k > 0)
            if(!writeHistSepLine(day))
                return 0;

        for(j=0;j<k;j++)
        {
            if(!delFlight(bbAux, idFlightForHist[j]))
                return 0;
        }
    }
    
    f = fopen(confDay,"w");
    
    if(f==NULL)
    {
        printf("Error occured while changing day on the file!\n");
        return 0;
    }
    fprintf(f,"%d",day);
    fclose(f);
    return 1;
}

int delCity(pBackbone bbAux, char *cityName)
{
    pCity delCity;
    pCity cAux;
    pCity cAuxNextCity;
    
    delCity = malloc(sizeof(Cities));
    cAux = malloc(sizeof(Cities));
    cAuxNextCity = malloc(sizeof(Cities));
    
    if(delCity == NULL || cAux == NULL || cAuxNextCity == NULL)
    {
        printf("Allocation error: Removing city!\n");
        return 0;
    }

    cAux = bbAux->fCity;
    cAuxNextCity = cAux->nCity;
    
    if(isCityListEmpty(bbAux->fCity))
    {
        return 0;
    }
    
    if(checkCityFlight(bbAux,cityName))
    {
        return 0;
    }
    
    if((delCity=findCityv2(bbAux,cityName)) == NULL)
    {
        return 0;
    }
    else {
        if(bbAux->nCities == 1)
        {
            bbAux->fCity = NULL;
            bbAux->lCity = NULL;
            bbAux->nCities = 0;
            free(delCity);
        } else {
            if (bbAux->nCities == 2)
            {
                if(cAux == delCity) /*1 pos*/
                {
                    bbAux->fCity = cAuxNextCity;
                    free(cAux);
                } else {
                    bbAux->lCity = cAux;
                    free(cAuxNextCity);
                }
                bbAux->nCities = 1;
            } else {
                if(strcmp(cAux->cityName,delCity->cityName) == 0)
                {
                     bbAux->fCity = cAuxNextCity;
                     free(cAux);
                } else {
                    while(cAux != NULL)
                    {
                        if(strcmp(cAuxNextCity->cityName,delCity->cityName) == 0)
                        {
                            cAux->nCity = cAuxNextCity->nCity;
                            
                            if(cAux->nCity == NULL)
                                bbAux->lCity = cAux;
                            
                            free(cAuxNextCity);
                            break;
                        }
                        cAux = cAux->nCity;
                        cAuxNextCity = cAux->nCity;
                    }
                }
                bbAux->nCities = bbAux->nCities - 1;
            }
        }
    }
    return 1;
}


pCity findCity(int idCity, pCity fCity)
{
    pCity cityAux = malloc(sizeof(Cities));
    cityAux = fCity;
    
    while(cityAux != NULL)
    {
        if(cityAux->idCity == idCity)
            return cityAux;
        cityAux = cityAux->nCity;
    }
    return NULL;
}

pCity findCityByName(char *cityName, pCity fCity)
{
    while(fCity != NULL)
    {
        if(strcmp(fCity->cityName,cityName) == 0)
            return fCity;
        fCity = fCity->nCity;
    }
    return NULL;
}

int findFlight(int idFlight, pBackbone bbAux)
{
    pFlight fAux = bbAux->fFlight;
    
    while(fAux != NULL)
    {
        if(fAux->idFlight == idFlight)
            return 1;
        fAux = fAux->nFlight;
    }
    return 0;
}

int addFlightv2(pBackbone bbAux,int idFlight, char *departure, char *arrival, int date)
{
    pFlight newFlight;
    pCity cDeparture;
    pCity cArrival;
    
    pFlight fAux = bbAux->fFlight;
    
    newFlight = malloc(sizeof(Flights));
    
    if(newFlight == NULL)
    {
        printf("Allocation error: New flight!\n");
        return 0;
    }
    
    if(date >= bbAux->date)
    {
        if(!findFlight(idFlight, bbAux))
        {
            if((cDeparture = findCityByName(departure, bbAux->fCity)) != NULL)
            {
                if((cArrival = findCityByName(arrival, bbAux->fCity)) != NULL)
                {
                    if(isFlightListEmpty(fAux))
                        bbAux->fFlight = newFlight;
                    else
                        bbAux->lFlight->nFlight = newFlight;
                    
                    bbAux->lFlight = newFlight;
                    newFlight->idFlight = idFlight;
                    newFlight->departure = cDeparture;
                    newFlight->arrival = cArrival;
                    newFlight->date = date;
                    newFlight->nFlight = NULL;
                    bbAux->nFlights = bbAux->nFlights + 1;
                    return 1;
                }
            }
        }
    }
    return 0;
}

pFlight addFlight(pBackbone bbAux, pFlight flightList, int idFlight, pCity cDeparture, pCity cArrival, int date)
{
    pFlight flightAux;
    
    flightAux = malloc(sizeof(Flights));
    
    if(flightAux == NULL)
    {
        printf("Allocation error: Aux Flight!\n");
        return flightList;
    }
    
    if(date >= bbAux->date)
    {
       if(isFlightListEmpty(flightList))
            bbAux->fFlight = flightAux;
       else
            flightList->nFlight = flightAux;
    
       flightAux->idFlight = idFlight;
       flightAux->departure = cDeparture;
       flightAux->arrival = cArrival;
       flightAux->date = date;
       flightAux->nFlight = NULL;
       bbAux->nFlights = bbAux->nFlights + 1;
       bbAux->lFlight = flightAux;
       return flightAux;
    } else
        return flightList;
}