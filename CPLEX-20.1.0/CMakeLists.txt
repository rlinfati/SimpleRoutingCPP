if (CMAKE_SYSTEM_NAME MATCHES "Darwin")

set(CPLEX_LIB ${CMAKE_CURRENT_SOURCE_DIR}/cplex/lib/x86-64_osx/static_pic/libcplex.a)
set(CONCERT_LIB ${CMAKE_CURRENT_SOURCE_DIR}/concert/lib/x86-64_osx/static_pic/libconcert.a)
set(ILOCPLEX_LIB ${CMAKE_CURRENT_SOURCE_DIR}/cplex/lib/x86-64_osx/static_pic/libilocplex.a)
set(CPLEX_LID ${CPLEX_LIB})
set(CONCERT_LID ${CONCERT_LIB})
set(ILOCPLEX_LID ${ILOCPLEX_LIB})
set(CPLEX_DLL ${CMAKE_CURRENT_SOURCE_DIR}/cplex/bin/x86-64_osx/libcplex2010.dylib)
set(CPLEX_EXTRALIB "")

elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")

set(CPLEX_LIB ${CMAKE_CURRENT_SOURCE_DIR}/cplex/lib/x86-64_linux/static_pic/libcplex.a)
set(CONCERT_LIB ${CMAKE_CURRENT_SOURCE_DIR}/concert/lib/x86-64_linux/static_pic/libconcert.a)
set(ILOCPLEX_LIB ${CMAKE_CURRENT_SOURCE_DIR}/cplex/lib/x86-64_linux/static_pic/libilocplex.a)
set(CPLEX_LID ${CPLEX_LIB})
set(CONCERT_LID ${CONCERT_LIB})
set(ILOCPLEX_LID ${ILOCPLEX_LIB})
set(CPLEX_DLL ${CMAKE_CURRENT_SOURCE_DIR}/cplex/bin/x86-64_linux/libcplex2010.so)
set(CPLEX_EXTRALIB "pthread;m;dl")

elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")

set(CPLEX_LIB ${CMAKE_CURRENT_SOURCE_DIR}/cplex/lib/x64_windows_msvc14/stat_mda/cplex2010.lib)
set(CONCERT_LIB ${CMAKE_CURRENT_SOURCE_DIR}/concert/lib/x64_windows_msvc14/stat_mda/concert.lib)
set(ILOCPLEX_LIB ${CMAKE_CURRENT_SOURCE_DIR}/cplex/lib/x64_windows_msvc14/stat_mda/ilocplex.lib)
set(CPLEX_LID ${CMAKE_CURRENT_SOURCE_DIR}/cplex/lib/x64_windows_msvc14/stat_mdd/cplex2010.lib)
set(CONCERT_LID ${CMAKE_CURRENT_SOURCE_DIR}/concert/lib/x64_windows_msvc14/stat_mdd/concert.lib)
set(ILOCPLEX_LID ${CMAKE_CURRENT_SOURCE_DIR}/cplex/lib/x64_windows_msvc14/stat_mdd/ilocplex.lib)
set(CPLEX_DLL ${CMAKE_CURRENT_SOURCE_DIR}/cplex/bin/x64_win64/cplex2010.dll)
set(CPLEX_EXTRALIB "")

endif()

add_library(cplex-library STATIC IMPORTED GLOBAL)
set_target_properties(cplex-library PROPERTIES
    IMPORTED_LOCATION ${CPLEX_LIB}
    IMPORTED_LOCATION_DEBUG ${CPLEX_LID}
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/cplex/include
    INTERFACE_LINK_LIBRARIES "${CPLEX_EXTRALIB}")

add_library(cplex-concert STATIC IMPORTED GLOBAL)
set_target_properties(cplex-concert PROPERTIES
    IMPORTED_LOCATION ${CONCERT_LIB}
    IMPORTED_LOCATION_DEBUG ${CONCERT_LID}
    INTERFACE_COMPILE_DEFINITIONS IL_STD
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/concert/include)

add_library(ilocplex STATIC IMPORTED GLOBAL)
set_target_properties(ilocplex PROPERTIES
    IMPORTED_LOCATION ${ILOCPLEX_LIB}
    IMPORTED_LOCATION_DEBUG ${ILOCPLEX_LID}
    INTERFACE_LINK_LIBRARIES "cplex-concert;cplex-library")

file(COPY ${CPLEX_DLL} DESTINATION ${CMAKE_BINARY_DIR})

