# requires minimum cmake version
cmake_minimum_required(VERSION 2.8.9)

# produce the cmake var PROJECT_NAME
project(tutorial_RFModule-simple)

# mandatory use of these packages
find_package(YARP 3.0 REQUIRED)
find_package(ICUBcontrib REQUIRED)

# extend the current search path used by cmake to load helpers
list(APPEND CMAKE_MODULE_PATH ${ICUBCONTRIB_MODULE_PATH})

# helpers defining certain macros
include(ICUBcontribHelpers)

# specify to install in $ICUBcontrib_DIR/bin
icubcontrib_set_default_prefix()

# building 
add_executable(tutorial_RFModule-simple ${CMAKE_SOURCE_DIR}/main.cpp)
target_link_libraries(tutorial_RFModule-simple ${YARP_LIBRARIES})
install(TARGETS tutorial_RFModule-simple DESTINATION bin)

icubcontrib_add_uninstall_target()



