cmake_minimum_required(VERSION 3.5.1)
project(dpd_externals CXX C)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-Wall -pedantic -Wextra")

#Bring the headers into the project
include_directories(./include)

#Add the sources, file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "./src/*.cc")

#Generate the shared library from the sources
add_library(dpd_externals SHARED ${SOURCES})
 
#Link the target to other libraries, if any
target_link_libraries(dpd_externals armadillo m)

#Set the location for library installation
install(TARGETS dpd_externals DESTINATION /usr/local/lib)

#Copy header files to the include path
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION /usr/local/include/dpd_externals FILES_MATCHING PATTERN "*.h")
