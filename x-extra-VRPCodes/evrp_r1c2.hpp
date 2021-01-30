#ifndef evrp_r1c2_hpp
#define evrp_r1c2_hpp

#include "evrp.h"
extern "C" int lkh_main(int argc, char *argv[]);

void eVRPSr1c2(Solution &s, SolutionData &sd);

void TSPWriteData(std::string sd, std::string sp, std::string ss);
void TSPCallLKH(std::string sp);
void TSPReadSolution(route &s, std::string ss);

#endif /* evrp_r1c2_hpp */
