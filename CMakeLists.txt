# ========================== Begin Init Stuff =============================
cmake_minimum_required(VERSION 3.14)
project(Mangine)
include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(LOCAL_FILES
    tests/unit_tests.cpp
    utils/Log.cpp
    utils/Config.cpp
    pipeline/Pipeline.cpp
)
set(LOCAL_DIRECTORIES
    tests
    utils
    libs
    pipeline
)
# ========================== End Init Stuff =================================

# ========================== Begin Library Stuff ============================
# GTest
FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.x
)

# Make sure to download GoogleTest
FetchContent_MakeAvailable(gtest)
# ========================== End Library Stuff ==============================

# ========================== Begin Compilation Stuff ========================
add_executable(rungt ${LOCAL_FILES})
target_include_directories(rungt PRIVATE ${LOCAL_DIRECTORIES})
target_link_libraries(rungt gtest gtest_main)
# ========================== End Compilation Stuff ==========================