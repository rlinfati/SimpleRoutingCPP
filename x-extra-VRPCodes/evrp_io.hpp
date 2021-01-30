#ifndef evrp_io_hpp
#define evrp_io_hpp

#include "evrp.h"

void eVRPRandomData(int t = 2532, int d = 1, int c = 25, int s = 5, int v = 3);
void eVRPDistance();
void eVRPCleanUp();
void eVRPWrite(std::string f);
void eVRPRead(std::string f);
void eVRPWriteGML(std::string f);

void eVRPPrintRoute(Solution &s, SolutionData &sd);
void eVRPCheckRouteData(Solution &s, SolutionData &sd);
void eVRPEvalSolution(Solution &s, SolutionData &sd, double &zval);
void eVRPWriteGML(std::string f, Solution &s);

#endif /* evrp_io_hpp */
