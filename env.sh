#!/bin/sh
SOFT_LOCAL="/home/yslian/local"

#################
#      GCC      #
#################
export GCC_VER="10.4.0"
export GCC_DIR="${SOFT_LOCAL}/gcc/v${GCC_VER}"
export PATH="${GCC_DIR}/bin:${PATH}"
export LD_LIBRARY_PATH="${GCC_DIR}/lib64:${LD_LIBRARY_PATH}"
export CC=`which gcc`
export CXX=`which g++`

#################
#     CMake     #
#################
export CMAKE_VER="3.24.0"
export CMAKE_DIR="${SOFT_LOCAL}/cmake/v${CMAKE_VER}/install"
export PATH="${CMAKE_DIR}/bin:${PATH}"
export LD_LIBRARY_PATH="${CMAKE_DIR}/lib:${LD_LIBRARY_PATH}"

#################
#      Qt6      #
#################
#export QT6_VER="6.3.1"
#export QT6_DIR="${SOFT_LOCAL}/qt6/v${QT6_VER}"
#export PATH="${QT6_DIR}/bin:${PATH}"
#export LD_LIBRARY_PATH="${QT6_DIR}/lib:${LD_LIBRARY_PATH}"

#################
#     ROOT      #
#################
source /home/yslian/local/ROOT/v6.24.06/install/bin/thisroot.sh
