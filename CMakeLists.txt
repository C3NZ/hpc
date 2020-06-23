cmake_minimum_required(VERSION 3.0)

# Handle build output
if (${DISTRIBUTION_BUILD})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Dist/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Dist/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Dist/bin)
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release" OR ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)
endif()

# Set env variables.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(BUILD_TESTING OFF)

project(app)

file(
    GLOB_RECURSE
    APP_SOURCE
    ${CMAKE_SOURCE_DIR}/src/*.h
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(app ${APP_SOURCE})

target_compile_options(
    app
    PUBLIC -mfpmath=sse
    PUBLIC -mmmx
    PUBLIC -msse
    PUBLIC -msse2
    PRIVATE -Wall 
    ${OpenMP_CXX_FLAGS}
)

add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/Vc)
target_link_libraries(app PUBLIC Vc)

find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)
find_package(
    Boost 
    REQUIRED 
    COMPONENTS serialization mpi
)

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost Not found")
endif()

target_link_libraries(
    app 
    PUBLIC OpenMP::OpenMP_CXX
    PUBLIC ${MPI_CXX_LIBRARIES}
    PRIVATE ${Boost_LIBRARIES}
)

target_include_directories(
    app
    PUBLIC ${Vc_INCLUDE_DIR}
    PUBLIC ${MPI_INCLUDE_PATH}
    PUBLIC ${Boost_INCLUDE_DIR}
)
