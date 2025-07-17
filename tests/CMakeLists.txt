# Add Windows install deployment for all test modules
include(${CMAKE_SOURCE_DIR}/cmake/WindowsInstall.cmake)
add_subdirectory(${CMAKE_SOURCE_DIR}/modules/GalaxyFactories/tests ${CMAKE_BINARY_DIR}/GalaxyFactoriesTests)
