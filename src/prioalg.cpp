#include "communication.hpp"
#include "mission.hpp"
#include <iostream>
#include "node.hpp"

using namespace std;

class IntegerClass
{
public:
    // Constructor
    IntegerClass(int value) {}

    // This method  is to calculate priority and return an integer value that represents a priority status
    int calculatePriority(float battery_charge, float min_charge)
    {

        // Initialize variables bat_pt and min_pt
        int bat_pt = 0;
        int min_pt = 0;

        // Conditions based on bat and min values
        if (battery_charge <= 1 && min_charge <= 5)
        {
            bat_pt = 100;
            min_pt = 60;
        }
        else if (battery_charge <= 1 && min_charge > 5)
        {
            bat_pt = 100;
            min_pt = 30;
        }
        else if (battery_charge > 1 && battery_charge <= 5 && min_charge <= 5)
        {
            bat_pt = 60;
            min_pt = 60;
        }
        else if (battery_charge > 1 && battery_charge <= 5 && min_charge > 5)
        {
            bat_pt = 60;
            min_pt = 30;
        }
        else if (battery_charge > 5 && battery_charge <= 10 && min_charge <= 5)
        {
            bat_pt = 20;
            min_pt = 60;
        }
        else if (battery_charge > 5 && battery_charge <= 10 && min_charge > 5)
        {
            bat_pt = 20;
            min_pt = 30;
        }
        else
        {
            bat_pt = 0;
            min_pt = 30;
        }

        // Output the results
        cout << "bat_pt: " << bat_pt << endl;
        cout << "min_pt: " << min_pt << endl;

        int prioValue = (bat_pt * 0, 6) + (min_pt * 0, 4);
        return prioValue;
    }
};