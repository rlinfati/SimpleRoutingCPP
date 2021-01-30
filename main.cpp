#include <iostream>
#include <cstdlib>
#include "routingl.h"

void test_tsp();
void test_vrp();
void test_cplex();
void test_gurobi();

int main(int argc, char *argv[]) {
    std::cerr << "-- SuperRouting build " <<  __DATE__ << " " << __TIME__ << std::endl;
    std::cerr << "-- BUILD: " << 8 * sizeof(void *) << "bits" << std::endl;
#ifdef NDEBUG
    std::cout << "-- Release version" << std::endl;
#endif
#ifdef _DEBUG
    std::cout << " - Debug version" << std::endl;
#endif
    std::cout.setf(std::ios_base::fixed);

    test_tsp();
    test_vrp();
    test_cplex();
    test_gurobi();

    return EXIT_SUCCESS;
}
