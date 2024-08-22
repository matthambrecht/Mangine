#!/bin/bash

START_DIR=$(PWD)

# Installs required system packages
package_install() {
    sudo apt install -y cmake python3-pip libpython3-dev
}

# Installs requirements and environment for embedding endpoint
embedding_install () {
    cd embedding
    python3 -m venv venv
    source venv/bin/activate

    pip3 install -U spacy
    python3 -m spacy download en_core_web_sm
    chmod +x entrypoint.sh

    cd $START_DIR
}

install_requirements () {
    package_install
    embedding_install
}

install_requirements