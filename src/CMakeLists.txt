cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(QtCommon)
include(GetGitVersion)

# Set additional project information

find_package(Qt5Core REQUIRED)

# Getting our repository version
get_git_version(GIT_VERSION)
string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" VERSION ${GIT_VERSION})

message(STATUS "Generating build using CMake v${CMAKE_VERSION}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Get source files list
file(GLOB SRC_LIST
    *.cpp
    )
file(GLOB H_LIST
    *.h    
    )

file(GLOB RC_LIST *.qrc)

#add_project_meta(META_FILES_TO_INCLUDE)

add_executable(readstdout ${OS_BUNDLE} ${SRC_LIST}
    ${H_LIST} ${RC_LIST}
    ${RESOURCE_FILES}
    )
target_link_libraries(readstdout Qt5::Core util)
