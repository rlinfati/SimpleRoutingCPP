#include "routingl.h"

void test_tsp() {

    int NTSPProblema = 100;
    std::string FTSPpro("TEST_TSP_Problem.txt");
    std::string FTSPpar("TEST_TSP_Parameter.txt");
    std::string FTSPsol("TEST_TSP_Solution.txt");
    std::string FTSPgml("TEST_TSP_Grafico.gml");

    std::vector<TSPNode> MyTSPPoints;
    std::vector <std::vector <double> > MyTSPDistances;
    std::vector <int> MyTSPRoute;

    RoutingL::TSPRandomData(MyTSPPoints, NTSPProblema);
    RoutingL::TSPDistance(MyTSPPoints, MyTSPDistances);
    RoutingL::TSPWriteData(MyTSPPoints, FTSPpro, FTSPpar, FTSPsol);

    RoutingL::TSPCallLKH(FTSPpar);

    RoutingL::TSPReadSolution(MyTSPRoute, FTSPsol);

    RoutingL::TSPWriteGraph(MyTSPPoints, MyTSPRoute, FTSPgml);
}
