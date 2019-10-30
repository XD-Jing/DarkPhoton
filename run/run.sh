#!/usr/bin/env bash

jobID=$1
NumEvtPerJob=100000

workdir=/home/lijing/DarkPhoton
source ${workdir}/setup.sh

mkdir -p ${workdir}/run/job${jobID}
cd ${workdir}/run/job${jobID}

cp /home/lijing/DarkPhoton/DarkPhoton/macros/example.in ./example.${jobID}

sed -i "s/ParaRndSed/$RANDOM/g" example.${jobID}
sed -i "s/ParaNumEvt/${NumEvtPerJob}/g" example.${jobID}

simu example.${jobID}

SlimROOT
