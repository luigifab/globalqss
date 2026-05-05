#!/bin/bash
# Debian: sudo apt install build-essential cmake qtbase5-dev qt6-base-dev
# Fedora: sudo dnf install cmake qt5-qtbase-devel qt6-qtbase-devel
# openSUSE: sudo zypper install cmake libqt5-qtbase-devel qt6-base-devel

rm -rf src-*/builder/

cd src-5/builder/
cmake .. && make
cd ../..

cd src-6/builder/
cmake .. && make
cd ../..

ls -altrh src-5/builder/ src-6/builder/