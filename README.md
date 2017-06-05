# IMtrajectoryModel
A tool that converts heater states (e.g. from a logfile) to a trajectory.

## Dependencies
There are no dependencies right now.

## Getting Started
To build the tool, just run
```
cd IMtrajectoryModel
make
```
To make sure that everything works as expected, you can run the tests by
```
./IMtrajectoryModel --test
```
or
```
./IMtrajectoryModel -t
```
In order to use IMtrajectoryModel to calculate a melting trajectory from an IceMole logfile you can run
```
./IMtrajectoryModel -l LOGFILE -i INPUTFILE -o OUTPUTFILE
```
in which `LOGFILE` is the location of the IceMole logfile, `INPUTFILE` the location of the input file (e.g. inputs.ini) and `OUTPUTFILE` the location where you want to save the melting trajectory.
