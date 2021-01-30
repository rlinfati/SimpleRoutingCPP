#ifndef evrp_sweep_hpp
#define evrp_sweep_hpp

#include "evrp.h"

struct angleTheta {
    int id;
    double theta;
    angleTheta(int i, double d) : id(i), theta(d) {}
    bool operator<(const angleTheta& b) const { return (this->theta < b.theta); }
    bool operator>(const angleTheta& b) const { return (this->theta > b.theta); }
};

void eVRPSweep(Solution &s, SolutionData &sd);

#endif /* evrp_sweep_hpp */
