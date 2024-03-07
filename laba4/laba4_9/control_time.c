#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "structures.h"

void add_time(my_time* tm)
{
        if (tm->min == 59) 
        {
            tm->min = 0;
            if (tm->hour == 23)
            {
                tm->hour = 0;
                if (tm->day == 30)
                {
                    tm->day = 1;
                    if(tm->mounth == 12)
                    {
                        tm->year++;
                        tm->mounth = 1;
                    }
                    else tm->mounth++;
                }
                else tm->day++;
            }
            else tm->hour++;
        }
        else tm->min++;
   
}
void fprint_time(FILE* file, my_time* tm, int* seek)
{
    //[2024-03-06T11:04:41]
    *seek += fprintf(file, "[");
    *seek += fprintf(file, "%d-", tm->year);
    if (tm->mounth < 10)  *seek += fprintf(file, "0%d-", tm->mounth);
    else *seek += fprintf(file, "%d-", tm->mounth);
    if (tm->day < 10) *seek += fprintf(file, "0%dT", tm->day);
    else *seek += fprintf(file, "%dT", tm->day);
    if (tm->hour < 10) *seek += fprintf(file, "0%d:", tm->hour);
    else *seek += fprintf(file, "%d:", tm->hour);
    if (tm->min < 10) *seek += fprintf(file, "0%d:", tm->min);
    else *seek += fprintf(file, "%d:", tm->min);
    if (tm->second < 10) *seek += fprintf(file, "0%d", tm->second);
    else *seek += fprintf(file, "%d] ", tm->second);
}

int does_time_finish(my_time* currentTime, my_time* finishTime)
{
    if (currentTime->year > finishTime->year)
        return 1;
    else if (currentTime->year < finishTime->year)
        return 0;
    
    if (currentTime->year == finishTime->year) 
    {
        if (currentTime->mounth > finishTime->mounth)
            return 1;
        else if (currentTime->mounth < finishTime->mounth)
            return 0;

        if (currentTime->day > finishTime->day)
            return 1;
        else if (currentTime->day < finishTime->day)
            return 0;

        if (currentTime->hour > finishTime->hour)
            return 1;
        else if (currentTime->hour < finishTime->hour)
            return 0;

        if (currentTime->min > finishTime->min)
            return 1;
        else if (currentTime->min < finishTime->min)
            return 0;

        if (currentTime->second > finishTime->second)
            return 1;
        else if (currentTime->second <= finishTime->second)
            return 0;
    }
    
    return 0;
}