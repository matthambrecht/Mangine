#!/bin/bash

if ! command -v jq 2>&1 >/dev/null
then
    sudo apt-get install -y jq
    exit 1
fi

START_DIR=$(pwd)
DB_NAME=$(cat config.json | jq -r ".database.db_name")
DB_USER=$(cat config.json | jq -r ".database.db_user")
DB_PASS=$(cat config.json | jq -r ".database.db_pass")

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
    sudo apt install -y cmake python3-pip libpython3-dev libssl-dev libcurl4-openssl-dev

    cd embedding
    python3 -m venv venv
    source venv/bin/activate

    pip3 install spacy pyyaml uvicorn fastapi 

    python3 -m spacy download en_core_web_md
    chmod +x entrypoint.sh

    cd $START_DIR
}

# Installs the PostgreSQL libs
db_install () {
    cd install

    sudo apt-get -y install postgresql postgresql-server-dev-all

    # Install pgvector lib
    git clone --branch v0.7.3 https://github.com/pgvector/pgvector.git
    cd pgvector
    make
    sudo make install
    cd ..

    # Install pqxx lib
    git clone --branch 7.9.2 https://github.com/jtv/libpqxx.git
    cd libpqxx
    CXXFLAGS=-std=c++17 ./configure
    make
    sudo make install
    cd ..

    # Install pgvector-cpp lib
    git clone --branch v0.2.0 https://github.com/pgvector/pgvector-cpp.git
    cd pgvector-cpp
    cmake -S . -B build
    cmake --build build
    cmake --install build

    cd $START_DIR
}

postgres_setup() {
    sudo -u postgres psql -c "CREATE ROLE ${DB_USER} WITH SUPERUSER LOGIN PASSWORD '${DB_PASS}';"
    sudo -u postgres psql -c "CREATE DATABASE ${DB_NAME}"
    sudo -u postgres psql -c "GRANT ALL PRIVILEGES ON DATABASE ${DB_NAME} TO ${DB_USER}"
}

install_requirements () {
    mkdir -p install

    package_install
    embedding_install
    cpr_install
    json_install
    gtest_install
    db_install
    postgres_setup
}

install_requirements