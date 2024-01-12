#include "communication.hpp"
#include "mission.hpp"
#include <iostream>
#include "node.hpp"
/*
Development notes:
kyl: true or false 
last : last vaierar mellan 0.5 - 5.5 
*/
using namespace std;

// This method  is to calculate priority and return an integer value that represents a priority status
float calculatePriority(float battery_charge, float min_charge, Mission mission)
{
    float bat_pt = 0;
    float min_pt = 0;
    float last_p = 0;
    float kyl_p = 0;

    int last_pt = mission.last; // 0-10 ton
    bool kyl_pt = mission.kylvara; // true or false 
    
    // För last och kylvara 
    if (last_pt >= 0)
    {
        if (last_pt = 0){
            last_p == 0;
        }
        else if (last_pt = 1) {
            last_p == 10;
        }
        else if (last_pt = 2) {
            last_p == 20;
        }
        else if (last_pt = 3) {
            last_p == 30;
        }
        else if (last_pt = 4) {
            last_p == 40;
        }
        else if (last_pt = 5) {
            last_p== 50;
        }
        
        else if (last_pt = 6) {
            last_p == 60;
        }
        
        else if (last_pt = 7) {
            last_p == 70;
        }
        
        else if (last_pt = 8) {
            last_p == 80;
        }
        
        else if (last_pt = 9) {
            last_p == 90;

        }
        
        else if (last_pt = 10) {
            last_p == 100;
        }
    }

    if (kyl_pt == true)
    {
        // lägger till poäng när de e kylvara 
        kyl_p += 100;
    }
    else {
        kyl_p == 0;
    }

    if (battery_charge <= 1)
    {
        if (min_charge <= 9.0)
        {
            bat_pt = 100;
            min_pt = 100;
        }
        else if (min_charge <= 13.5)
        {
            bat_pt = 100;
            min_pt = 60;
        }
        else if (min_charge <= 27.0)
        {
            bat_pt = 100;
            min_pt = 40;
        }
        else if (min_charge <= 54.0)
        {
            bat_pt = 100;
            min_pt = 20;
        }
        else if (min_charge <= 99.0)
        {
            bat_pt = 100;
            min_pt = 0;
        }
    }
    else if (battery_charge <= 5)
    {
        if (min_charge <= 13.5)
        {
            bat_pt = 50;
            min_pt = 75;
        }
        else if (min_charge <= 27.0)
        {
            bat_pt = 50;
            min_pt = 50;
        }
        else if (min_charge <= 54.0)
        {
            bat_pt = 50;
            min_pt = 25;
        }
        else if (min_charge <= 99.0)
        {
            bat_pt = 50;
            min_pt = 0;
        }
    }
    else
    {
        if (min_charge <= 27.0)
        {
            bat_pt = 0;
            min_pt = 60;
        }
        else if (min_charge <= 54.0)
        {
            bat_pt = 0;
            min_pt = 30;
        }
        else if (min_charge <= 99.0)
        {
            bat_pt = 0;
            min_pt = 0;
        }
    }
    

    float prioValue = (bat_pt * 0.4) + (min_pt * 0.2) + (last_p *0.1) + (kyl_p * 0.3);
    return prioValue;
}

