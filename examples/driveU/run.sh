#!/bin/bash
cd ${0%/*} || exit 1    # run from this directory
../../build/IMtrajectoryModel -sl driveU.csv -el 10000
../../build/IMtrajectoryModel -sl driveU_ext.csv -i inputs_driveU.ini -o result.log