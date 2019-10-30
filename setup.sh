###
###  Envrionment for Dark Photon Simulation
###
source /home/zhangyulei/utility.sh


# DIR="$( cd "$( dirname "${BASH_SOURCE[1]}" )" >/dev/null 2>&1 && pwd )"
# echo $DIR $PWD $(dirname $(readlink -f $0))
DIR=$(dirname $(readlink -f $0))

function compile(){
    currentDir=$(pwd);
    cd ${DIR}/DarkPhoton/build;
    make;
    cd ${currentDir};
}

printf "${BBlue}=%0.s" $( seq 1 30 )
printf "\n%25s%15s\n" "Dark Photon Simulation" " "
printf "%20s%15s\n" "@ INPAC && TDLI" " "
printf "=%0.s" $( seq 1 30 )
echo -e "${ColorEnd}"
printfmt="     %-10s%-10s\n"
printclr="     ${IRed}%-10s${IRed}%-10s${ColorEnd}\n"


#--------------------------------------------------------------------------------
#     DP_simu
#--------------------------------------------------------------------------------
#export DP_DIR="/home/zhangyulei/dark_photon/Simulation/DP_simu"
export DP_DIR="/home/lijing/DarkPhoton/DarkPhoton"
export PATH="${DP_DIR}/build:$PATH"

printf "$printclr" "DP_simu" "0.1.0"

#--------------------------------------------------------------------------------
#     HepMC-2.6.9
#--------------------------------------------------------------------------------
export HEPMC_DIR="/home/zhangyulei/softwares/HepMC/install"
export LD_LIBRARY_PATH="${HEPMC_DIR}/lib"

printf "$printclr" "HepMC" "2.6.9"

#--------------------------------------------------------------------------------
#     gcc-4.9.4
#--------------------------------------------------------------------------------
export CC="${GCC_BIN}/gcc"
export CXX="${GCC_BIN}/g++"
export PATH="/home/zhangyulei/softwares/CEPC/cepcsoft/x86_64-sl6-gcc49/external/GCC/4.9.4/install/bin:$PATH"
export GCC_BIN="/home/zhangyulei/softwares/CEPC/cepcsoft/x86_64-sl6-gcc49/external/GCC/4.9.4/install/bin"
export GCC_HOME="/home/zhangyulei/softwares/CEPC/cepcsoft/x86_64-sl6-gcc49/external/GCC/4.9.4/"
export GCC_LIB="/home/zhangyulei/softwares/CEPC/cepcsoft/x86_64-sl6-gcc49/external/GCC/4.9.4/install/lib64"
export LD_LIBRARY_PATH="${GCC_HOME}/install/lib:${GCC_LIB}:${LD_LIBRARY_PATH}"

printf "$printfmt" "GCC" "`gcc --version | head -n1 | tr " " "\n" | tail -n1`"

#--------------------------------------------------------------------------------
#     CMake
#--------------------------------------------------------------------------------
export PATH="/home/zhangyulei/softwares/cmake-3.14.4/install/bin:$PATH"

printf "$printfmt" "CMake" "`cmake --version | head -n1 | tr " " "\n" | tail -n1`"

#--------------------------------------------------------------------------------
#     ROOT 6.16
#--------------------------------------------------------------------------------
export ROOTSYS="/home/zhangyulei/softwares/Root-6.16/install/"
#export ROOTSYS="/home/zhangyulei/softwares/Root-6.06.08/install/"
export PATH="$ROOTSYS/bin:$PATH"
export LD_LIBRARY_PATH="$ROOTSYS/lib:$LD_LIBRARY_PATH"
export CMAKE_PREFIX_PATH="${ROOTSYS}:$CMAKE_PREFIX_PATH"
export CMAKE_MODULE_PATH="${ROOTSYS}/cmake:$CMAKE_MODULE_PATH"
source ${ROOTSYS}/bin/thisroot.sh

printf "$printclr" "ROOT" "`root-config --version`"

#--------------------------------------------------------------------------------
#     Python 3.6.8
#--------------------------------------------------------------------------------
export PYTHONPATH="/home/zhangyulei/softwares/Python/install3"
#export PYTHONPATH="/home/zhangyulei/softwares/Python-2.7.15"
export PATH="$PYTHONPATH/bin:$PATH"
export LD_LIBRARY_PATH="$PYTHONPATH/lib:$LD_LIBRARY_PATH"
export CMAKE_PREFIX_PATH=$PYTHONPATH:$CMAKE_PREFIX_PATH



printf "$printfmt" "Python" "`python --version | tr " " "\n" | tail -n1`"

#--------------------------------------------------------------------------------
#     QT
#--------------------------------------------------------------------------------
export QT_BIN="/home/zhangyulei/softwares/CEPC/cepcsoft/x86_64-sl6-gcc49/external/QT/4.7.4/install/bin"
export QT_HOME="/home/zhangyulei/softwares/CEPC/cepcsoft/x86_64-sl6-gcc49/external/QT/4.7.4/install"
export QT_INCLUDE="/home/zhangyulei/softwares/CEPC/cepcsoft/x86_64-sl6-gcc49/external/QT/4.7.4/install/include"
export QT_LIB="/home/zhangyulei/softwares/CEPC/cepcsoft/x86_64-sl6-gcc49/external/QT/4.7.4/install/lib"
export PATH="${QT_HOME}/bin:$PATH"
export LD_LIBRARY_PATH="${QT_LIB}:$LD_LIBRARY_PATH"
export CMAKE_PREFIX_PATH="${QT_HOME}:$CMAKE_PREFIX_PATH"

#--------------------------------------------------------------------------------
#     Geant4
#--------------------------------------------------------------------------------
export G4INSTALL="/home/zhangyulei/softwares/Geant4-10.05/install/"
export G4ENV_INIT="/home/zhangyulei/softwares/Geant4-10.05/install/bin/geant4.sh"
export G4SYSTEM="Linux-g++"
export CMAKE_PREFIX_PATH="${G4INSTALL}:$CMAKE_PREFIX_PATH"
#source ${G4ENV_INIT}
#source /home/lijing/dark_photon/geant4.sh



#-----------------------------------------------------------------------
# Bourne shell environment setup script for Geant4 10.5.1
#
# This script will configure your environment so that any Geant4 supplied
# tools, libraries and data are available in your PATH, library paths etc.
# Data libraries will only be setup if they were installed as part of the
# Geant4 installation.
#
# Source this script to perform the setup.
#
# This script is autogenerated by CMake DO NOT EDIT
#

#-----------------------------------------------------------------------
# Locate directory of self
#
# Self locate script when sourced
#if [ -z "$BASH_VERSION" ]; then
#  # Not bash, so rely on sourcing from correct location
#  #if [ ! -f geant4.sh ]; then
#  #  echo 'ERROR: geant4.sh could NOT self-locate Geant4 installation'
#  #  echo 'This is most likely because you are using ksh, zsh or similar'
#  #  echo 'To fix this issue, cd to the directory containing this script'
#  #  echo 'and source it in that directory.'
#  #  return 1
#  #fi
#  #geant4_envbindir=$(pwd)
#  geant4_envbindir="/home/zhangyulei/softwares/Geant4-10.05/install/bin"
#else
#  g4sls_sourced_dir=$(dirname ${BASH_ARGV[0]})
#  geant4_envbindir=$(cd $g4sls_sourced_dir > /dev/null ; pwd)
#fi

 geant4_envbindir="/home/zhangyulei/softwares/Geant4-10.05/install/bin"

#-----------------------------------------------------------------------
# Setup Geant4 binary and library paths...
#

if test "x$PATH" = "x" ; then
  export PATH="$geant4_envbindir"
else
  export PATH="$geant4_envbindir":${PATH}
fi


if test "x$LD_LIBRARY_PATH" = "x" ; then
  export LD_LIBRARY_PATH="`cd $geant4_envbindir/../lib64 > /dev/null ; pwd`"
else
  export LD_LIBRARY_PATH="`cd $geant4_envbindir/../lib64 > /dev/null ; pwd`":${LD_LIBRARY_PATH}
fi


#-----------------------------------------------------------------------
# Setup Third-party binary and library paths...
# - CLHEP
# - Builtin CLHEP used

# - XercesC

if test "x$LD_LIBRARY_PATH" = "x" ; then
  export LD_LIBRARY_PATH=/usr/lib64
else
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/lib64
fi


#-----------------------------------------------------------------------
# Resource file paths
# - Datasets
export G4NEUTRONHPDATA="`cd $geant4_envbindir/../share/Geant4-10.5.1/data/G4NDL4.5 > /dev/null ; pwd`"
export G4LEDATA="`cd $geant4_envbindir/../share/Geant4-10.5.1/data/G4EMLOW7.7 > /dev/null ; pwd`"
export G4LEVELGAMMADATA="`cd $geant4_envbindir/../share/Geant4-10.5.1/data/PhotonEvaporation5.3 > /dev/null ; pwd`"
export G4RADIOACTIVEDATA="`cd $geant4_envbindir/../share/Geant4-10.5.1/data/RadioactiveDecay5.3 > /dev/null ; pwd`"
export G4PARTICLEXSDATA="`cd $geant4_envbindir/../share/Geant4-10.5.1/data/G4PARTICLEXS1.1 > /dev/null ; pwd`"
export G4PIIDATA="`cd $geant4_envbindir/../share/Geant4-10.5.1/data/G4PII1.3 > /dev/null ; pwd`"
export G4REALSURFACEDATA="`cd $geant4_envbindir/../share/Geant4-10.5.1/data/RealSurface2.1.1 > /dev/null ; pwd`"
export G4SAIDXSDATA="`cd $geant4_envbindir/../share/Geant4-10.5.1/data/G4SAIDDATA2.0 > /dev/null ; pwd`"
export G4ABLADATA="`cd $geant4_envbindir/../share/Geant4-10.5.1/data/G4ABLA3.1 > /dev/null ; pwd`"
export G4INCLDATA="`cd $geant4_envbindir/../share/Geant4-10.5.1/data/G4INCL1.0 > /dev/null ; pwd`"
export G4ENSDFSTATEDATA="`cd $geant4_envbindir/../share/Geant4-10.5.1/data/G4ENSDFSTATE2.2 > /dev/null ; pwd`"


# - Fonts for Freetype
# FREETYPE SUPPORT NOT AVAILABLE

#----------------------------------------------------------------------

printf "$printclr" "Geant4" "`geant4-config --version`"
