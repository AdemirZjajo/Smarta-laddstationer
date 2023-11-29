#ifndef INTEGER_CLASS_HPP
#define INTEGER_CLASS_HPP

class IntegerClass {
public:
    // Constructor
    IntegerClass(int value);

    // Method to calculate priority and return an integer value that represents a priority status
    int calculatePriority(float battery_charge, float min_charge);

private:
    // Add any private members here if needed
};

#endif // INTEGER_CLASS_HPP