#include <gurobi_c++.h>

void test_gurobi() {
    GRBEnv* env = 0;
    try {
        env = new GRBEnv();
        GRBModel model = GRBModel(*env);
        std::cout << "-- GUROBI VERSION: " << GRB_VERSION_MAJOR << "." << GRB_VERSION_MINOR << "." << GRB_VERSION_TECHNICAL << std::endl;
    } catch (GRBException e) {
        std::cerr << "!! ErrorCode: " << e.getErrorCode() << std::endl;
        std::cerr << "!! ErrorMsg: " << e.getMessage() << std::endl;
    } catch (...) {
        std::cerr << "!! Error" << std::endl;
    }
    delete env;
}
