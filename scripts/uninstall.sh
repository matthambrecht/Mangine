#!/bin/bash

rm -rf embedding/venv embedding/__pycache__
rm -rf install
rm -rf utils/json.hpp
make clean_all
sudo rm -rf /usr/local/include/curl /usr/local/include/cpr /usr/local/include/gtest /usr/local/include/gmock /usr/local/include/pgvector
sudo systemctl stop postgresql
sudo apt-get remove -y --purge postgresql postgresql-*
sudo rm -rf /var/lib/postgresql/*
sudo rm -rf /etc/postgresql
sudo rm -rf /etc/pgsql