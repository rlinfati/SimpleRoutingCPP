#include "routingl.h"

void test_vrp() {

    int NVRPProblema = 100;
    std::string FVRPpro("TEST_VRP_Problem.txt");
    std::string FVRPini("TEST_VRP_Inicial.txt");
    std::string FVRPsol("TEST_VRP_Solution.txt");
    std::string FVRPgml("TEST_VRP_Grafico.gml");

    std::vector<VRPNode> MyVRPPoints;
    int MyVRPVehicleCap = 50.0*NVRPProblema/20.0;
    std::vector <std::vector <double> > MyVRPDistances;
    std::vector <std::vector <int> > MyVRPRoute;

    RoutingL::VRPRandomData(MyVRPPoints, NVRPProblema);
    RoutingL::VRPDistance(MyVRPPoints, MyVRPDistances);
    RoutingL::VRPWriteData(MyVRPPoints, MyVRPVehicleCap, FVRPpro);

    RoutingL::VRPCallVRPH(FVRPpro, FVRPsol, std::string() );

    RoutingL::VRPReadSolution(MyVRPRoute, FVRPsol);
    RoutingL::VRPWriteSolution(MyVRPRoute, FVRPini, NVRPProblema);

    RoutingL::VRPCallVRPH(FVRPpro, FVRPsol, FVRPini);

    RoutingL::VRPWriteGraph(MyVRPPoints, MyVRPRoute, FVRPgml);
}
