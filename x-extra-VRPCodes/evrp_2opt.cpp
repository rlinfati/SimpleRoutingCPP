#include <iostream>
#include <cmath>
#include "evrp_2opt.hpp"

void eVRP2opt(Solution &s, SolutionData &sd, int t, Solution &sout, SolutionData &sdout, TabuGranularAnnealing &tg) {
    TRACEFUNC;
    
    sout = s;
    sdout = sd;

    double bestdelta = 0.0001;
    int movtype = -1; // 1 tsp-2opt
    int movr1 = -1;
    int movr2 = -1;
    int movr1p1 = -1;
    int movr1p2 = -1;
    
    for (int r1 = 0; r1 < (int)s.size(); ++r1) {
        for (int r1p1 = 0; r1p1 + 1 < (int)s[r1].size(); ++r1p1) {
            for (int r1p2 = r1p1 + 2; r1p2 + 1 < (int)s[r1].size(); ++r1p2) {
                
                if ( tg.isTabu( s[r1][r1p1] , s[r1][r1p2] )) continue;
                if ( tg.isTabu( s[r1][r1p1+1] , s[r1][r1p2+1] )) continue;

                double r1_rlen = DISTANCE[ s[r1][r1p1] ][ s[r1][r1p1+1] ] + DISTANCE[ s[r1][r1p2] ][ s[r1][r1p2+1] ]
                - DISTANCE[ s[r1][r1p1] ][ s[r1][r1p2] ] - DISTANCE[ s[r1][r1p1+1] ][ s[r1][r1p2+1] ];
                
                if (r1_rlen > bestdelta) {
                    bestdelta = r1_rlen;
                    movtype = 1;
                    movr1 = movr2 = r1;
                    movr1p1 = r1p1;
                    movr1p2 = r1p2;
                    if (t == 1) goto ApplyMovExit;
                } else {
                    if (tg.typesa == 10) {
                        double r = rand()/double(RAND_MAX);
                        double delta = r1_rlen;
                        if ( r < exp( -delta / tg.sa_t ) ) {
                            bestdelta = r1_rlen;
                            movtype = 1;
                            movr1 = movr2 = r1;
                            movr1p1 = r1p1;
                            movr1p2 = r1p2;
                            if (t == 1) goto ApplyMovExit;
                        }
                    }
                }
            }
        }
        for (int r2 = r1+1; r2 <= (int)s.size(); ++r2) {
            // TODO: distinta ruta
        }
    }
    
ApplyMovExit:
    switch (movtype) {
        case 1:
            std::reverse(sout[movr1].begin()+movr1p1+1, sout[movr1].begin()+movr1p2+1);
            
            sdout[movr1].rlen = 0;
            for (size_t j = 0; j+1 < sout[movr1].size(); ++j) {
                sdout[movr1].rlen += DISTANCE[ sout[movr1][j] ][ sout[movr1][j+1] ];
            }
            
            tg.addTabu(s[movr1][movr1p1] , s[movr1][movr1p2]);
            tg.addTabu(s[movr1][movr1p1+1] , s[movr1][movr1p2+1]);
            tg.cleanTabu();
            
            std::cout << __FUNCTION__ << " *movtype " << movtype << " "
            << movr1 << "-" << movr1p1 << "x" << movr1p2
            << " delta " << bestdelta
            << std::endl;
            break;
            
        case -1:
        default:
            std::cout << __FUNCTION__ << " *movtype " << movtype << " not improvement" << std::endl;
            break;
    }
}
