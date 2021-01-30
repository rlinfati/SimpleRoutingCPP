#include <iostream>
#include "evrp_initialsolution.hpp"

#include "evrp_io.hpp"

#include "evrp_saving.hpp"
#include "evrp_sweep.hpp"
#include "evrp_r1c2.hpp"
#include "evrp_vrph.hpp"

void eVRPInitialSolution(Solution &s, SolutionData &sd, int t) {
        TRACEFUNC;
    
    switch (t) {
        case 1:
            eVRPSaving(s, sd);
            break;

        case 2:
            eVRPSweep(s, sd);
            break;

        case 3:
            eVRPSr1c2(s, sd);
            break;

        case 4:
            eVRPvrph(s, sd);
            break;

        case -1:
        default:
            std::cout << __FUNCTION__ << " *type " << t << " not implemented" << std::endl;
            break;
    }
    
    eVRPPrintRoute(s, sd);
    eVRPCheckRouteData(s, sd);
    eVRPWriteGML("evrp-test-initial.gml", s);
}


