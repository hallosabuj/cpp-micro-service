cmake_minimum_required(VERSION 3.8)

project(RestServer C CXX)

########################################
# Creating shared liabrary
########################################
include_directories("include")
set(REST_SOURCES 
    src/server.cpp
)
add_library(rest_server SHARED ${REST_SOURCES})

# Generating executable
add_executable(server main.cpp)
target_link_libraries(server PRIVATE rest_server boost_system boost_thread pthread)