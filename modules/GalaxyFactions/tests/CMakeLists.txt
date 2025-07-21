cmake_minimum_required(VERSION 3.24)

# Include Google Test
include(googletest)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create test executable for GalaxyFactions
add_executable(GalaxyFactionsTests
    test_resource_type.cpp
    test_system_resource_bonus.cpp
    test_system_notes.cpp
    test_handover_type.cpp
    test_faction_handover.cpp
    test_system.cpp
    test_faction.cpp
    test_galaxy_factions.cpp
)

target_link_libraries(GalaxyFactionsTests PRIVATE
    gtest_main
    GGH::Galaxy::Factions
)

# Add test to CTest
gtest_discover_tests(GalaxyFactionsTests)
