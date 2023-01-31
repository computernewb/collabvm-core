cmake_minimum_required(VERSION 3.19)

# Catch some invalid things that people could do.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
	message(FATAL_ERROR "In-source builds are strictly prohibited.")
endif()

include(cmake/Policies.cmake)

project(collabvm-core
		VERSION 1.0.0
		DESCRIPTION "CollabVM Core library"
		)

include(CTest)
include(FetchContent)

include(cmake/CollabVM.cmake)

# Dependencies of the top level projects
find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)

# Top level third party dependencies
set(EXPECTED_BUILD_TESTS OFF)
add_subdirectory(third_party/expected)

# Use an alternative linker for the project
if(NOT "${COLLABVM_LINKER}" STREQUAL "")
	collabvm_core_set_alternate_linker(${COLLABVM_LINKER})
endif()

add_subdirectory(src)

