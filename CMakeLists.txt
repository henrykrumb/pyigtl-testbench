cmake_minimum_required(VERSION 3.14)

project(pyigtltestbench)
include(FetchContent)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

FetchContent_Declare(
  OpenIGTLink
  GIT_REPOSITORY	https://github.com/openigtlink/OpenIGTLink
  # was the most recent commit hash on 19.04.2023
  GIT_TAG			d4eaae937b6a28ea2324f90c128800b3ad1cf2b3
)
FetchContent_MakeAvailable(OpenIGTLink)

include(FetchContent)
FetchContent_Declare(
  cli11
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11
  GIT_TAG        v2.3.2
)
FetchContent_MakeAvailable(cli11)

# build igtlink server
find_package(OpenIGTLink REQUIRED)
include(${OpenIGTLink_USE_FILE})
include_directories(${cli11_SOURCE_DIR}/include)



add_executable(testbench_server source/testbench_server.cpp)
target_link_libraries(testbench_server OpenIGTLink)

add_executable(testbench_client source/testbench_client.cpp)
target_link_libraries(testbench_client OpenIGTLink)

set_target_properties(testbench_server testbench_client
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin"
)
