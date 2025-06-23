cmake_minimum_required(VERSION 3.12)
project(UberFavoriteDriverFeature VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /Zi")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

# Include directories
include_directories(cpp/include)

# Source files
set(SOURCES
    cpp/src/Driver.cpp
    cpp/src/FavoriteDriverManager.cpp
    cpp/src/RideRequest.cpp
)

# Header files
set(HEADERS
    cpp/include/Driver.h
    cpp/include/FavoriteDriverManager.h
    cpp/include/RideRequest.h
)

# Create library
add_library(UberFavoriteDriver STATIC ${SOURCES} ${HEADERS})

# Create test executable (optional)
option(BUILD_TESTS "Build test executable" ON)
if(BUILD_TESTS)
    add_executable(UberFavoriteDriverTest cpp/tests/main.cpp)
    target_link_libraries(UberFavoriteDriverTest UberFavoriteDriver)
endif()

# Create example executable
option(BUILD_EXAMPLES "Build example executable" ON)
if(BUILD_EXAMPLES)
    add_executable(UberFavoriteDriverExample cpp/examples/main.cpp)
    target_link_libraries(UberFavoriteDriverExample UberFavoriteDriver)
endif()

# Installation
install(TARGETS UberFavoriteDriver
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${HEADERS}
    DESTINATION include/uber
)

# Package configuration
set(CPACK_PACKAGE_NAME "UberFavoriteDriverFeature")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ implementation of Uber Favorite Driver Feature")
set(CPACK_PACKAGE_VENDOR "Your Name")
set(CPACK_PACKAGE_CONTACT "your.email@example.com")

include(CPack)
