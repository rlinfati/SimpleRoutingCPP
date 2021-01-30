#include <iostream>
#include "evrp_annealing.hpp"

#include "evrp_io.hpp"

#include "evrp_swap.hpp"
#include "evrp_2opt.hpp"

void eVRPSimAnnealing(Solution &s, SolutionData &sd, int t, Solution &sout, SolutionData &sdout) {
    TRACEFUNC;
    
    std::vector<void (*)(Solution &, SolutionData &, int, Solution &, SolutionData &, TabuGranularAnnealing &)> neighborhood;
    neighborhood.push_back(eVRPSwap);
    neighborhood.push_back(eVRP2opt);
    
    sout = s;
    sdout = sd;
    TabuGranularAnnealing tg;
    tg.typesa = 10;
    
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
            
            neighborhood[i](sout, sdout, 1, currentS, currentSD, tg);
            eVRPEvalSolution(currentS, currentSD, currentZ);
            
            // eVRPCheckRouteData(currentS, currentSD);
            // eVRPPrintRoute(currentS, currentSD);
            
            bestZ = currentZ;
            bestS = currentS;
            bestSD = currentSD;
            std::cout << __FUNCTION__ << " neighborhood " << i << " newz " << currentZ << std::endl;
            if (t == 1) break;
        }
        
        if (v % tg.sa_down == 0) tg.sa_t = tg.sa_t * tg.sa_alpha;
        sout = bestS;
        sdout = bestSD;
        std::cout << __FUNCTION__ << std::endl;
    }
    
    // TODO: arreglar annealing... muy random
    
    eVRPCheckRouteData(sout, sdout);
    eVRPPrintRoute(sout, sdout);
    eVRPWriteGML("evrp-test-sa.gml", sout);
}
