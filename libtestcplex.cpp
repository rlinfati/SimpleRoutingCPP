#include <ilcplex/ilocplex.h>

void test_cplex() {
    IloEnv env;
    try {
        IloCplex cplex(env);
        std::cout << "-- CPLEX VERSION: " << cplex.getVersion() << std::endl;
        std::cout << "-- CONCERT VERSION: " << env.getVersion() << std::endl;
    } catch (IloException& ex) {
        std::cerr << "!! Error: " << ex << std::endl;
    } catch (...) {
        std::cerr << "!! Error" << std::endl;
    }
    env.end();
}
