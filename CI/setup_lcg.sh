#!/bin/sh -ex
#
# setup LCG release via cvmfs

# determine os release
if [ "$(cat /etc/redhat-release | grep 'Scientific Linux CERN SLC release 6')" ]; then
  os=slc6
  compiler=gcc62-opt
elif [ "$(cat /etc/centos-release | grep 'CentOS Linux release 7')" ]; then
  os=centos7
  compiler=gcc8-opt
else
  echo "Unknown OS" 1>&2
  exit 1
fi

release=LCG_95
platform=x86_64-${os}-${compiler}
lcg=/cvmfs/sft.cern.ch/lcg/views/${release}/${platform}

source ${lcg}/setup.sh

