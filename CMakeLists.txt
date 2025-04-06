# ========================== Begin Init Stuff =============================
cmake_minimum_required(VERSION 3.14)
project(Mangine)
include(FetchContent)
include(ExternalProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(TEST_FLAGS 
    -Wall
    -Wextra
    -pedantic
    -Werror
    -g)

set(SERVICE_TEST_FILES
    tests/service_unit_tests.cpp
    src/utils/Log.cpp
    src/indexer/TextProcessor.cpp
    src/indexer/Man.cpp
    src/indexer/Indexer.cpp
    src/database/Database.cpp
    src/database/Corpora.cpp
    src/interface/API.cpp
    src/interface/Parser.cpp
)
set(SERVICE_TEST_DIRECTORIES
    tests
    src/utils
    include
    src/indexer
    src/database
    src/interface
)

set(SERVICE_FILES
    src/utils/Log.cpp
    src/indexer/TextProcessor.cpp
    src/indexer/Man.cpp
    src/indexer/Indexer.cpp
    src/database/Database.cpp
    src/database/Corpora.cpp
    src/interface/API.cpp
    src/interface/service.cpp
    src/interface/Parser.cpp
)
set(SERVICE_DIRECTORIES
    tests
    src/utils
    include
    pipeline
    src/indexer
    database
    interface
)

set(CLIENT_FILES
    src/interface/client.cpp
)
set(CLIENT_DIRECTORIES
    interface
    include
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

# ========================== Begin Service Test Compilation Stuff ========================
add_executable(service_gt ${SERVICE_TEST_FILES})
add_compile_options(${TEST_FLAGS})
target_include_directories(service_gt PRIVATE ${SQLITE3_INCLUDE_DIR} ${SERVICE_TEST_DIRECTORIES})
target_link_libraries(service_gt gtest gtest_main ${SQLITE3_LIBRARIES})
add_dependencies(service_gt sqlite3)
# ========================== End Service Test Compilation Stuff ==========================

# ========================== Begin Client Compilation Stuff ========================
add_executable(client ${CLIENT_FILES})
add_compile_options(${TEST_FLAGS})
target_include_directories(client PRIVATE ${CLIENT_DIRECTORIES})
add_dependencies(client sqlite3)
# ========================== End Client Compilation Stuff ==========================

# ========================== Begin Service Compilation Stuff ========================
add_executable(service ${SERVICE_FILES})
add_compile_options(${TEST_FLAGS})
target_include_directories(service PRIVATE ${SQLITE3_INCLUDE_DIR} ${SERVICE_DIRECTORIES})
target_link_libraries(service ${SQLITE3_LIBRARIES})
add_dependencies(service sqlite3)
# ========================== End Service Compilation Stuff ==========================