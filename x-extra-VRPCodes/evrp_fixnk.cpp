#include <iostream>
#include <algorithm>
#include <climits>
#include "evrp_io.hpp"
#include "evrp_fixnk.hpp"

void eVRPFixnk(Solution &s, SolutionData &sd) {
    TRACEFUNC;
    
    std::vector<int> skipc(CUSTOMERS.size());
    while ((int)s.size() > (int)VEHICLES.size()) {
        // imr = iter ruta con menor demanda
        // idxmr = n de imr
        SolutionData::iterator imr = std::min_element(sd.begin(), sd.end(),
                                                      [](routeData cl, routeData cc) {
                                                          return cl.rdem < cc.rdem;
                                                      });
        size_t idxmr = std::distance(sd.begin(), imr);
        
        // imd = cliente con mayor demanda en la ruta imr
        route::iterator imd = std::max_element(s[idxmr].begin(), s[idxmr].end(),
                                               [&skipc](int cl, int cc) {
                                                   if (skipc[cc]) return false;
                                                   return CUSTOMERS[cl].dem < CUSTOMERS[cc].dem;
                                               });
        
        // idxdr = n de ruta con mayor capacidad libre
        int idxdr = -1;
        int idxdrv = INT_MIN;
        for (int i = 0; i < (int)s.size(); ++i) {
            if (i == (int)idxmr) continue;
            int capLibre = VEHICLES[0].cap - sd[i].rdem;
            if (capLibre < CUSTOMERS[*imd].dem) continue;
            
            if (capLibre > idxdrv) {
                idxdr = i;
                idxdrv = capLibre;
            }
        }
        
        if (NODES[*imd].type != eVRPNode::customer) break;
        
        if ( idxdr == -1 ) {
            skipc[*imd] = true;
            continue;
        }
        
        int pi = 0;
        double piv = __DBL_MAX__;
        for (int i = 0; i + 1 < (int)s[idxdr].size(); ++i) {
            double inc = DISTANCE[ s[idxdr][i] ][ *imd ]
            + DISTANCE[ *imd ][ s[idxdr][i+1] ]
            - DISTANCE[ s[idxdr][i] ][ s[idxdr][i+1] ];
            
            if (inc < piv) {
                pi = i+1;
                piv = inc;
            }
        }
        
        std::cout << "FixNK move c" << *imd << " a la ruta r" << idxdr << std::endl;
        
        sd[idxdr].rlen += piv;
        sd[idxdr].rdem += CUSTOMERS[*imd].dem;
        s[idxdr].insert(s[idxdr].begin() + pi, *imd);
        
        sd[idxmr].rlen += DISTANCE[ *(imd-1) ][ *(imd+1) ] - DISTANCE[ *(imd-1) ][ *imd ] - DISTANCE[ *imd ][ *(imd+1) ];
        sd[idxmr].rdem -= CUSTOMERS[*imd].dem;
        s[idxmr].erase( imd );
        
        for (int i = 0; i < (int)s.size(); ++i) {
            if (sd[i].rdem) continue;
            s.erase( s.begin() + i );
            sd.erase( sd.begin() + i );
        }
    }
    
    eVRPCheckRouteData(s, sd);
    eVRPPrintRoute(s, sd);
    eVRPWriteGML("evrp-test-fixnk.gml", s);
    
    // TODO: decidir si partir con exceso de rutas o solucion infactible
}
