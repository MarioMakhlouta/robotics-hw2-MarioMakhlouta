#!/bin/bash
g++ -std=c++17 -Wall -Wextra -pthread src/*.cpp main.cpp -I include -o fleet_app
