#!/bin/bash
# Debian: sudo apt install build-essential cmake qtbase5-dev qt6-base-dev
# Fedora: sudo dnf install cmake qt5-qtbase-devel qt6-qtbase-devel
# openSUSE: sudo zypper install cmake libqt5-qtbase-devel qt6-base-devel

cd "$(dirname "$0")" || exit 1
rm -rf src/builder-qt*/

mkdir src/builder-qt5
cd src/builder-qt5/
cmake -DQT_MAJOR=5 .. && make
cd ../..

mkdir src/builder-qt6
cd src/builder-qt6/
cmake -DQT_MAJOR=6 .. && make
cd ../..

ls -altrh src/builder-qt5/ src/builder-qt6/