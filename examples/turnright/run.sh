#!/bin/bash
cd ${0%/*} || exit 1    # run from this directory
../../build/IMtrajectoryModel -sl turnright_toUse.csv -i inputs_turnright.ini -o result.log
