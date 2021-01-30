add_library(vrphlocal STATIC
src/ClarkeWright.cpp
src/Concatenate.cpp
src/CrossExchange.cpp
src/Flip.cpp
src/MoveString.cpp
src/OnePointMove.cpp
src/OrOpt.cpp
src/Postsert.cpp
src/Presert.cpp
src/RNG.cpp
src/Swap.cpp
src/SwapEnds.cpp
src/Sweep.cpp
src/ThreeOpt.cpp
src/ThreePointMove.cpp
src/TwoOpt.cpp
src/TwoPointMove.cpp
src/VRP.cpp
src/VRPDebug.cpp
src/VRPGenerator.cpp
src/VRPGraphics.cpp
src/VRPIO.cpp
src/VRPMove.cpp
src/VRPNode.cpp
src/VRPRoute.cpp
src/VRPSolution.cpp
src/VRPSolvers.cpp
src/VRPTSPLib.cpp
src/VRPTabuList.cpp
src/VRPUtils.cpp)

target_include_directories(vrphlocal PUBLIC inc)

add_executable(vrp_initial EXCLUDE_FROM_ALL src/apps/vrp_initial.cpp)
target_link_libraries(vrp_initial vrphlocal)

add_executable(vrp_rtr EXCLUDE_FROM_ALL src/apps/vrp_rtr.cpp)
target_link_libraries(vrp_rtr vrphlocal)

add_executable(vrp_sa EXCLUDE_FROM_ALL src/apps/vrp_sa.cpp)
target_link_libraries(vrp_sa vrphlocal)
