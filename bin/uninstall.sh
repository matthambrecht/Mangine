#!/bin/bash

rm -rf embedding/venv embedding/__pycache__
rm -rf install
rm -rf utils/json.hpp
make clean_all
sudo rm -rf /usr/local/include/curl /usr/local/include/cpr /usr/local/include/gtest /usr/local/include/gmock