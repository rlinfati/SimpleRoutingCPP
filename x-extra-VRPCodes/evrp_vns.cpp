#include <iostream>
#include "evrp_vns.hpp"

#include "evrp_io.hpp"

#include "evrp_swap.hpp"
#include "evrp_2opt.hpp"

void eVRPVNS(Solution &s, SolutionData &sd, int t, Solution &sout, SolutionData &sdout) {
    TRACEFUNC;
    
    std::vector<void (*)(Solution &, SolutionData &, int, Solution &, SolutionData &, TabuGranularAnnealing &)> neighborhood;
    neighborhood.push_back(eVRPSwap);
    neighborhood.push_back(eVRP2opt);
    
    sout = s;
    sdout = sd;
    TabuGranularAnnealing tg;
    int nni = 0;
    
    for (int v = 0; v < 10; ++v) { // TODO: numero de iteraciones
        double bestZ = __DBL_MAX__;
        Solution bestS = sout;
        SolutionData bestSD = sdout;
        eVRPEvalSolution(bestS, bestSD, bestZ);
        std::cout << __FUNCTION__ << " vuelta " << v << " nni " << nni << " z " << bestZ << std::endl;
        
        double currentZ = __DBL_MAX__;
        Solution currentS;
        SolutionData currentSD;
        
        neighborhood[nni](sout, sdout, t, currentS, currentSD, tg);
        eVRPEvalSolution(currentS, currentSD, currentZ);
        
        // eVRPCheckRouteData(currentS, currentSD);
        // eVRPPrintRoute(currentS, currentSD);
        
        if (currentZ < bestZ) {
            bestZ = currentZ;
            bestS = currentS;
            bestSD = currentSD;
            std::cout << __FUNCTION__ << " neighborhood " << nni << " newz " << currentZ << std::endl;
            if (t == 1) break;
        } else {
            nni = nni + 1 >= (int)neighborhood.size() ? 0 : nni+1;
        }
        sout = bestS;
        sdout = bestSD;
    }
    
    eVRPCheckRouteData(sout, sdout);
    eVRPPrintRoute(sout, sdout);
    eVRPWriteGML("evrp-test-vns.gml", sout);
    
}
