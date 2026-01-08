#!/bin/bash

#Checking for Updates and installing Essential tools
sudo apt update -y
sudo apt install build-essential libcurl4-openssl-dev -y

#Running Makefile
make clean
make

