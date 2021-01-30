#include <iostream>
#include <cmath>
#include "evrp_swap.hpp"

void eVRPSwap(Solution &s, SolutionData &sd, int t, Solution &sout, SolutionData &sdout, TabuGranularAnnealing &tg) {
    TRACEFUNC;
    
    sout = s;
    sdout = sd;

    double bestdelta = 0.0001;
    int movtype = -1;
    int movr1 = -1;
    int movr2 = -1;
    int movr1p = -1;
    int movr2p = -1;
    
    for (int r1 = 0; r1 < (int)s.size(); ++r1) {
        for (int r1p = 1; r1p + 1 < (int)s[r1].size(); ++r1p) {
            for (int r2 = r1; r2 < (int)s.size(); ++r2) {
                for (int r2p = 1; r2p + 1 < (int)s[r2].size(); ++r2p) {
                    if (r1 == r2) if (r1p + 1 >= r2p) continue;

                    if ( tg.isTabu( s[r1][r1p-1] , s[r2][r2p] )) continue;
                    if ( tg.isTabu( s[r2][r2p] , s[r1][r1p+1] )) continue;
                    if ( tg.isTabu( s[r2][r2p-1] , s[r1][r1p] )) continue;
                    if ( tg.isTabu( s[r1][r1p] , s[r2][r2p+1]  )) continue;
                    
                    double r1_rdem = CUSTOMERS[ s[r1][r1p] ].dem - CUSTOMERS[ s[r2][r2p] ].dem;
                    double r2_rdem = CUSTOMERS[ s[r2][r2p] ].dem - CUSTOMERS[ s[r1][r1p] ].dem;

                    if (sd[r1].rdem - r1_rdem > VEHICLES[0].cap ) continue;
                    if (sd[r2].rdem - r2_rdem > VEHICLES[0].cap ) continue;
                    
                    double r1_rlen = DISTANCE[ s[r1][r1p-1] ][ s[r1][r1p] ] + DISTANCE[ s[r1][r1p] ][ s[r1][r1p+1] ]
                    - DISTANCE[ s[r1][r1p-1] ][ s[r2][r2p] ] - DISTANCE[ s[r2][r2p] ][ s[r1][r1p+1] ];
                    double r2_rlen = DISTANCE[ s[r2][r2p-1] ][ s[r2][r2p] ] + DISTANCE[ s[r2][r2p] ][ s[r2][r2p+1] ]
                    - DISTANCE[ s[r2][r2p-1] ][ s[r1][r1p] ] - DISTANCE[ s[r1][r1p] ][ s[r2][r2p+1] ];

                    if (r1_rlen + r2_rlen > bestdelta) {
                        bestdelta = r1_rlen + r2_rlen;
                        movtype = 1;
                        movr1 = r1;
                        movr2 = r2;
                        movr1p = r1p;
                        movr2p = r2p;
                        if (t == 1) goto ApplyMovExit;
                    } else {
                        if (tg.typesa == 10) {
                            double r = rand()/double(RAND_MAX);
                            double delta = r1_rlen + r2_rlen;
                            if ( r < exp( -delta / tg.sa_t ) ) {
                                bestdelta = r1_rlen + r2_rlen;
                                movtype = 1;
                                movr1 = r1;
                                movr2 = r2;
                                movr1p = r1p;
                                movr2p = r2p;
                                if (t == 1) goto ApplyMovExit;
                            }
                        }
                    }
                }
            }
        }
    }
    
ApplyMovExit:
    switch (movtype) {
        case 1:
            std::swap(sout[movr1][movr1p], sout[movr2][movr2p]);
            
            sdout[movr1].rlen = 0;
            sdout[movr1].rdem = 0;
            for (size_t j = 0; j+1 < sout[movr1].size(); ++j) {
                sdout[movr1].rlen += DISTANCE[ sout[movr1][j] ][ sout[movr1][j+1] ];
                sdout[movr1].rdem += CUSTOMERS[ sout[movr1][j] ].dem;
            }
            
            sdout[movr2].rlen = 0;
            sdout[movr2].rdem = 0;
            for (size_t j = 0; j+1 < sout[movr2].size(); ++j) {
                sdout[movr2].rlen += DISTANCE[ sout[movr2][j] ][ sout[movr2][j+1] ];
                sdout[movr2].rdem += CUSTOMERS[ sout[movr2][j] ].dem;
            }
            
            tg.addTabu(s[movr1][movr1p-1] , s[movr2][movr2p]);
            tg.addTabu(s[movr2][movr2p] , s[movr1][movr1p+1]);
            tg.addTabu(s[movr2][movr2p-1] , s[movr1][movr1p]);
            tg.addTabu(s[movr1][movr1p] , s[movr2][movr2p+1]);
            tg.cleanTabu();
                        
            std::cout << __FUNCTION__ << " *movtype " << movtype << " "
            << movr1 << "-" << movr1p << "x" << movr2 << "-" << movr2p
            << " delta " << bestdelta
            << std::endl;
            break;
            
        case -1:
        default:
            std::cout << __FUNCTION__ << " *movtype " << movtype << " not improvement" << std::endl;
            break;
    }
}
