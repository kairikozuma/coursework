// Demonstrate polymorphism and inheritance, pure virtual functiosn

#include <iostream>
#include <math.h>

using namespace std;

class Car {
    public:
        // Constructor
        Car() : position(0), velocity(0), acceleration(0) {}
        void print();

        // Have subclass implement the accerlate method
        // Pure virtual function
        virtual void accelerate(double a)  = 0;

        void updatePosition(double now);
        void advanceTime(double elapsed);
        bool stopped();
    private:
        double position;
        double velocity;
    // Protected, can be accessed directly by subclass
    protected:
        double acceleration;
}; // Do not forget semicolon

void Car::print() {
    cout << "Position is " << position
         << " velocity is " << velocity
         << " acceleration is " << acceleration << endl;
}

void Car::updatePosition(double elapsed) {
    double initVelocity = velocity;
    // New velocity = old velocity + acceleration * elapsed time
    velocity += acceleration * elapsed;
    // New position assumes average velocity over the acceleration period
    position += (initVelocity + (velocity - initVelocity)/2.0) * elapsed;
}

void Car::advanceTime(double elapsed) {
    // Advance time for car object
    updatePosition(elapsed);
    print();
}

bool Car::stopped() {
    // True if car is stopped, velocity is 0
    return (velocity == 0.0);
}

// Define Yugo subclass
class Yugo : public Car {
    public:
        // All subclasses of cars must implement accelerate
        virtual void accelerate(double a);
};

void Yugo::accelerate(double a) {
    // Yugo cannot accelerate more than 0.2 mps^2
    // If it exceeds, set to +- 0.2
    if (fabs(a) > 0.2) {
        a = 0.2 * a / fabs(a);
    }
    acceleration = a;
}

// Define Ferrari subclass
class Ferrari : public Car {
    public:
        // All subclasses of cars must implement accelerate
        virtual void accelerate(double a);
};

void Ferrari::accelerate(double a) {
    // Yugo cannot accelerate more than 10.0 mps^2
    // If it exceeds, set to +- 10.0
    if (fabs(a) > 10.0) {
        a = 10.0 * a / fabs(a);
    }
    acceleration = a;
}

int main() {
    // Car c // will not compile
    Yugo y;
    Ferrari f;

    y.accelerate(20);
    f.accelerate(20);

    y.advanceTime(60);
    f.advanceTime(60);
}
