#include <iostream>
#include "evrp_localsearch.hpp"

#include "evrp_io.hpp"

#include "evrp_swap.hpp"
#include "evrp_2opt.hpp"

void eVRPLocalSearch(Solution &s, SolutionData &sd, int t, Solution &sout, SolutionData &sdout) {
    TRACEFUNC;
    
    std::vector<void (*)(Solution &, SolutionData &, int, Solution &, SolutionData &, TabuGranularAnnealing &)> neighborhood;
    neighborhood.push_back(eVRPSwap);
    neighborhood.push_back(eVRP2opt);

    // evrp_istabu = &evrp_istabu_no;
    
    sout = s;
    sdout = sd;
    TabuGranularAnnealing tg;
    
    for (int v = 0; v < 10; ++v) { // TODO: numero de iteraciones
        double bestZ = __DBL_MAX__;
        Solution bestS = sout;
        SolutionData bestSD = sdout;
        eVRPEvalSolution(bestS, bestSD, bestZ);
        std::cout << __FUNCTION__ << " vuelta " << v << " z " << bestZ << std::endl;

        for (int i = 0; i < (int)neighborhood.size(); ++i) {
            double currentZ = __DBL_MAX__;
            Solution currentS;
            SolutionData currentSD;
            
            neighborhood[i](sout, sdout, t, currentS, currentSD, tg);
            eVRPEvalSolution(currentS, currentSD, currentZ);
            
            // eVRPCheckRouteData(currentS, currentSD);
            // eVRPPrintRoute(currentS, currentSD);
            
            if (currentZ < bestZ) {
                bestZ = currentZ;
                bestS = currentS;
                bestSD = currentSD;
                std::cout << __FUNCTION__ << " neighborhood " << i << " newz " << currentZ << std::endl;
                if (t == 1) break;
            }
        }
        sout = bestS;
        sdout = bestSD;
    }
    
    eVRPCheckRouteData(sout, sdout);
    eVRPPrintRoute(sout, sdout);
    eVRPWriteGML("evrp-test-localsearch.gml", sout);

}

