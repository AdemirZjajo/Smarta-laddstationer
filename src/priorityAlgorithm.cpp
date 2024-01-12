#include "communication.hpp"
#include "mission.hpp"
#include <iostream>
#include "node.hpp"

using namespace std;

// This method  is to calculate priority and return an integer value that represents a priority status
float calculatePriority(float battery_charge, float min_charge )
{
    float bat_pt = 0;
    float min_pt = 0;

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

    float prioValue = (bat_pt * 0.6) + (min_pt * 0.4);
    return prioValue;
}

