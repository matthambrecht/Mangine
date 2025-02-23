# ========================== Begin Init Stuff =============================
cmake_minimum_required(VERSION 3.14)
project(Mangine)
include(FetchContent)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(LOCAL_FILES
    tests/unit_tests.cpp
    utils/Log.cpp
    pipeline/Pipeline.cpp
    indexer/Man.cpp
    indexer/Indexer.cpp
    database/Database.cpp
)
set(LOCAL_DIRECTORIES
    tests
    utils
    libs
    pipeline
    indexer
    database
)
# ========================== End Init Stuff =================================

# ========================== Begin Library Stuff ============================
# GTest
FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.x
)

ExternalProject_Add(
  sqlite3
  GIT_REPOSITORY https://github.com/sqlite/sqlite.git
  GIT_TAG version-3.49.0
  SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/sqlite3-build"
  BINARY_DIR "${CMAKE_BINARY_DIR}/_deps/sqlite3-build"
  STAMP_DIR "${CMAKE_BINARY_DIR}/_deps/sqlite3-stamp"
  PREFIX ${CMAKE_BINARY_DIR}/_deps
  GIT_SHALLOW 1
  CONFIGURE_COMMAND "${CMAKE_BINARY_DIR}/_deps/sqlite3-build/configure"
  BUILD_COMMAND make
  UPDATE_COMMAND ""
  INSTALL_COMMAND ""
)

FetchContent_MakeAvailable(gtest)

# Specify where to find sqlite3 headers and library
set(SQLITE3_INCLUDE_DIR ${CMAKE_BINARY_DIR}/_deps/sqlite3-build/)
set(SQLITE3_LIBRARIES ${CMAKE_BINARY_DIR}/_deps/sqlite3-build/libsqlite3.a)
# ========================== End Library Stuff ==============================

# ========================== Begin Compilation Stuff ========================
add_executable(rungt ${LOCAL_FILES})
add_compile_options(-Wall -Wextra -pedantic -Werror -g)
target_include_directories(rungt PRIVATE ${SQLITE3_INCLUDE_DIR} ${LOCAL_DIRECTORIES})
target_link_libraries(rungt gtest gtest_main ${SQLITE3_LIBRARIES})
add_dependencies(rungt sqlite3)
# ========================== End Compilation Stuff ==========================