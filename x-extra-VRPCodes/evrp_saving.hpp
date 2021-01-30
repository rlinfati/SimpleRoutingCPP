#ifndef evrp_saving_hpp
#define evrp_saving_hpp

#include "evrp.h"

struct savingCost {
    int i;
    int j;
    double differ;
    savingCost(int a, int b, double s) : i(a), j(b), differ(s) {}
    bool operator<(const savingCost& b) const { return (this->differ < b.differ); }
    bool operator>(const savingCost& b) const { return (this->differ > b.differ); }
};

void eVRPSaving(Solution &s, SolutionData &sd);

#endif /* evrp_saving_hpp */
