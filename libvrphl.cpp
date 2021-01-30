#include <iostream>
#include "VRPH.h"

void vrphl(char *input=NULL, char *output=NULL, char *initial=NULL) {
    bool fruity_loop = true;
    bool enable_sa = true;
    bool enable_rtr = true;
    bool enable_ej = false;

    if (!input) return;

    double tini = clock();

    int n = VRPGetDimension(input);
    VRP V(n);
    V.read_TSPLIB_file(input);

    int num_ejected = n-2 < 20 ? n-2 : 20;
    int num_trials = 1000;

    double best_obj = -1;
    int *best_sol = new int[n+2];
    if (initial) {
        V.read_solution_file(initial);
        best_obj = V.get_total_route_length();
        V.export_solution_buff(best_sol);
        std::cout << "VRPH File " << V.get_total_route_length()
             << " time " << (clock()-tini)/CLOCKS_PER_SEC << std::endl;
    } else {
        ClarkeWright CW(n);
        CW.Construct(&V,
                     1.0, // lambda
                     false); // verbose
        best_obj = V.get_total_route_length();
        V.export_solution_buff(best_sol);
        std::cout << "VRPH ClarkeWright " << V.get_total_route_length()
             << " time " << (clock()-tini)/CLOCKS_PER_SEC << std::endl;
    }

    if (fruity_loop) std::cout << "VRPH fruity_loop mode" << std::endl;
    do {
        double current_sol = best_obj;

        do {
            if ( ! enable_sa ) break;
            V.SA_solve(ONE_POINT_MOVE+TWO_POINT_MOVE+TWO_OPT, // heuristics
                       2, // starting_temperature
                       .99, // cooling_ratio
                       2, // iters_per_loop
                       200, // num_loops
                       10, // nlist_size
                       false); // verbose
            if ( best_obj > V.get_total_route_length() ) {
                best_obj = V.get_total_route_length();
                std::cout << "VRPH SA " << V.get_total_route_length()
                     << " time " << (clock()-tini)/CLOCKS_PER_SEC << std::endl;
                V.export_solution_buff(best_sol);
            } else {
                V.import_solution_buff(best_sol);
                break;
            }
        } while (fruity_loop);

        do {
            if ( ! enable_rtr ) break;
            V.RTR_solve(ONE_POINT_MOVE+TWO_POINT_MOVE+TWO_OPT+VRPH_USE_NEIGHBOR_LIST, // heuristics
                        40, // intensity (inicial 30)
                        5, // max_tries
                        2, // num_perturbs
                        .01, // dev
                        40, // nlist_size
                        VRPH_LI_PERTURB, // perturb_type
                        VRPH_FIRST_ACCEPT, // accept_type
                        false); // verbose
            if ( best_obj > V.get_total_route_length() ) {
                best_obj = V.get_total_route_length();
                std::cout << "VRPH RTR " << V.get_total_route_length()
                     << " time " << (clock()-tini)/CLOCKS_PER_SEC << std::endl;
                V.export_solution_buff(best_sol);
            } else {
                V.import_solution_buff(best_sol);
                break;
            }
        } while (fruity_loop);

        do {
            if ( ! enable_ej ) break;
            double current_ej = best_obj;
            int *ejected_buff = new int[n+2];

            for(int i=0;i<num_trials;++i) {
                int r = VRPH_DEPOT;
                while(r == VRPH_DEPOT) r = (int)(lcgrand(11)*(n-1));

                V.eject_neighborhood(r, // j
                                     num_ejected, // num
                                     ejected_buff); // nodelist
                V.inject_set(num_ejected, // num
                             ejected_buff, // nodelist
                             VRPH_REGRET_SEARCH, // rules
                             50); // attempts

                if ( best_obj > V.get_total_route_length() ) {
                    best_obj = V.get_total_route_length();
                    std::cout << "VRPH EJ " << i << " Regret " << V.get_total_route_length()
                         << " time " << (clock()-tini)/CLOCKS_PER_SEC << std::endl;
                    V.export_solution_buff(best_sol);
                } else {
                    V.import_solution_buff(best_sol);
                }
            }
            delete[] ejected_buff;

            if (current_ej <= best_obj+1e-4) break;
        } while (fruity_loop);

        if (current_sol <= best_obj+1e-4) break;
    } while (fruity_loop);

    delete [] best_sol;
    std::cout << "* VRPH"
         << " route/best "
         << V.get_total_route_length() << "/" << V.get_best_known()
         << " time " << (clock()-tini)/CLOCKS_PER_SEC << std::endl;
    if (output) {
        V.write_solution_file(output);
    }
}   //fin de vrph

