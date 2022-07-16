project(sVRP)

cmake_minimum_required(VERSION 3.20)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT sVRP)

if(NOT CMAKE_BUILD_TYPE)
  # DEBUG RELWITHDEBINFO RELEASE
  set(CMAKE_BUILD_TYPE RELEASE)
endif()
message("-- CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

add_executable(sVRP
libtesttsp.cpp
libtestvrp.cpp
libtestcplex.cpp
libtestgurobi.cpp
libvrphl.cpp
routingl.cpp
main.cpp)

if(WIN32)
    message("-- WIN32 -- CFLAG/CXXFLAG: /Wall")
    target_compile_options(sVRP PRIVATE /Wall)
endif()

if(UNIX)
    message("-- UNIX -- CFLAG/CXXFLAG: -Wall -Wextra")
    target_compile_options(sVRP PRIVATE -Wall -Wextra)
    message("-- UNIX -- SKIP RPATH: TRUE")
    set(CMAKE_SKIP_RPATH TRUE)
endif()

if(APPLE)
    message("-- APPLE -- CFLAG/CXXFLAG: -Weverything")
    target_compile_options(sVRP PRIVATE -Weverything)
endif()

# sse  - nehalem     westmere          - corei7
# avx  - sandybridge ivybridge         - corei7-avx
# avx2 - haswell     broadwell skylake - core-avx2
# gcc -march=native -dM -E - < /dev/null | egrep "tune|SSE|AVX" | sort
# sudo yum install --enablerepo=codeready-builder-for-rhel-8-x86_64-rpms libstdc++-static

if(UNIX AND NOT APPLE)
    message("-- LINUX -- CFLAG/CXXFLAG:  -march=sandybridge -mtune=haswell")
    add_compile_options(                 -march=sandybridge -mtune=haswell)
    target_compile_options(sVRP PRIVATE  -march=sandybridge -mtune=haswell)
    message("-- LINUX -- STATIC libgcc and libstdc++")
    target_link_libraries(sVRP -static-libgcc -static-libstdc++)
endif()

add_subdirectory(LKH-3.0.3)
target_link_libraries(sVRP lkhlocal)

add_subdirectory(VRPH-1.0.0)
target_link_libraries(sVRP vrphlocal)

add_subdirectory(CPLEX-22.10)
target_link_libraries(sVRP ilocplex)

add_subdirectory(GUROBI-9.5)
target_link_libraries(sVRP gurobi-cpp)
