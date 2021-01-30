#ifndef evrp_vrph_hpp
#define evrp_vrph_hpp

#include "evrp.h"

void eVRPvrph(Solution &s, SolutionData &sd);

void vrphl(char *input=NULL, char *output=NULL, char *initial=NULL);

void VRPWriteData(std::string sd);
void VRPCallVRPH(std::string sp, std::string ss, std::string si);
void VRPReadSolution(Solution &s, std::string ss);

#endif /* evrp_vrph_hpp */
