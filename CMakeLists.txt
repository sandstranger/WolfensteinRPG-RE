cmake_minimum_required(VERSION 3.10)
project(WolfensteinRPG)

set(CMAKE_CXX_STANDARD 20)

if (NOT ANDROID)
	list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
else()
	set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")
endif()

if(MSVC)
	add_definitions("/D_CRT_SECURE_NO_WARNINGS")
endif()

find_package(SDL2 REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OpenAL REQUIRED)
find_package(OpenGL REQUIRED)

# Global identifiers for each project/target
set(HASH_LIBRARY_TGT_NAME           wolfesteinrpg-Hash-Library)

add_subdirectory("${PROJECT_SOURCE_DIR}/src")
add_subdirectory("${PROJECT_SOURCE_DIR}/third_party_libs/hash-library")