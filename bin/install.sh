#!/bin/bash

START_DIR=$(pwd)

# Installs required system packages
package_install() {
    sudo apt install -y cmake python3-pip libpython3-dev libssl-dev libcurl4-openssl-dev
}

# Installs google test for unit testing
gtest_install () {
    cd install
    git clone https://github.com/google/googletest.git -b v1.15.2
    cd googletest        
    mkdir build 
    cd build
    cmake ..  
    make
    sudo make install
    cd $START_DIR 
}

# Installs JSON Library for C++
json_install () {
    cd $START_DIR
    curl https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp > utils/json.hpp
}

# Instals Curl for People Library
cpr_install () {
    cd install

    git clone https://github.com/libcpr/cpr.git
    cd cpr && mkdir build && cd build
    cmake .. -DCPR_USE_SYSTEM_CURL=ON
    cmake --build . --parallel
    sudo cmake --install .
    cd $START_DIR
}

# Installs requirements and environment for embedding endpoint
embedding_install () {
    cd embedding
    python3 -m venv venv
    source venv/bin/activate

    pip3 install spacy pyyaml uvicorn fastapi 

    python3 -m spacy download en_core_web_sm
    chmod +x entrypoint.sh

    cd $START_DIR
}

install_requirements () {
    mkdir install

    # package_install
    # embedding_install
    # cpr_install
    # json_install
    gtest_install
}

install_requirements