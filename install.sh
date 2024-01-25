#!/bin/bash

pwd=$(pwd)

echo y | sudo dnt update
echo y | sudo dnf install gcc
sudo yum makecache
echo y | sudo yum groupinstall "Development Tools"
echo y | sudo yum install wget
cd /home/centos
wget https://github.com/Kitware/CMake/releases/download/v3.22.1/cmake-3.22.1.tar.gz
tar -zxvf cmake-3.22.1.tar.gz
cd cmake-3.22.1
./bootstrap
make
make install
echo alias=/usr/local/bin/cmake | ~/.bashrc

cd $pwd
mkdir build
cd build
cmake ..
make install