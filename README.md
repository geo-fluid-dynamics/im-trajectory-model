# IMtrajectoryModel
[![Build Status](https://travis-ci.org/geo-fluid-dynamics/im-trajectory-model.svg?branch=master)](https://travis-ci.org/geo-fluid-dynamics/im-trajectory-model)
A tool that converts heater state changes of the IceMole to a trajectory.

The heater state changes can be provided by two possible input files, namely a logfile and a simplified logfile. An example for a logfile is given by `test/logfile/icemole_state.log`. Examples for simplified logfiles can be found in `test/simplifiedLogfile/`.

Author: K. Schüller schueller@aices.rwth-aachen.de

## Dependencies
* [CMake](https://cmake.org)

## Getting Started
To build the tool, create a build fold in the root directoy
```
make build
cd build
```
Then execute
```
cmake ..
make
```
To make sure that everything works as expected, you can run the tests by
```
make test
```
Run
```
./IMtrajectoryModel -h
```
to see all possible input options.
In order to use IMtrajectoryModel to calculate a melting trajectory from an IceMole logfile you can run
```
./IMtrajectoryModel -l LOGFILE -i INPUTFILE -o OUTPUTFILE
```
in which `LOGFILE` is the location of the IceMole logfile, `INPUTFILE` the location of the input file (e.g. inputs.ini) and `OUTPUTFILE` the location where you want to save the melting trajectory.

## Coordinate System (COS)
### Melting Probe COS (local COS)
The IDs of the heating cartridges and wall heaters are used to define the melting probe COS as shown in the following schematic (note that the viewer looks from the back of the melting probe to the melting direction)

![IMschematic](doc/github/IMschematic.png)

### Ice COS (global COS)
The global COS (x-, y- and z-direction) is connected to the melting probe COS by the initial normal vector `n_0` and the initial tangent vector `t_0`, which can be set by the user. By default `n_0=(1,0,0)` and `t_0=(0,0,-1)`, which means that the local coordinates `X` and `Y` are aligned to the gloabal coordinates `x` and `y`. Further examples:
* n_0=(-1,0,0),t_0=(0,0,-1): X aligns with -x, Y aligns -y
* n_0=(0,1,0),t_0=(0,0,-1): X aligns with y, Y aligns -x
* n_0=(0,0,1),t_0=(1,0,0): X aligns with z, Y aligns y

## Heater States
| Heater Name | Position in heater state array |
| --- | --- |
| Head Heater 1 | 1 |
| Head Heater 2 | 2 |
| Head Heater 3 | 3 |
| Head Heater 4 | 4 |
| Head Heater 5 | 5 |
| Head Heater 6 | 6 |
| Head Heater 7 | 7 |
| Head Heater 8 | 8 |
| Head Heater 9 | 9 |
| Head Heater 10 | 10 |
| Head Heater 11 | 11 |
| Head Heater 12 | 12 |
| Head Heater 13 | 13 |
| Head Heater 14 | 14 |
| Head Heater 15 | 15 |
| Head Heater 16 | 16 |
| Wall Heater 1 | 17 |
| Wall Heater 2 | 18 |
| Wall Heater 3 | 19 |
| Wall Heater 4 | 20 |
| Wall Heater 5 | 21 |
| Wall Heater 6 | 22 |
| Wall Heater 7 | 23 |
| Wall Heater 8 | 24 |

## Postprocessing
The trajectory given in the output file after successful of IMtrajectoryModel is given by

| time | px | py | pz | tx | ty | tz | nx | ny | nz |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0 | px_0 | py_0 | pz_0 | tx_0 | ty_0 | tz_0 | nx_0 | ny_0 | nz_0 |
| ... | ... | ... | ... | ... | ... | ... | ... | ... | ... |

in which<br>
  **time**        is the time in seconds strating from 0<br>
  **px, py, pz**  is the position in meter<br>
  **tx, ty, tz**  is the tangential vector of the trajectory in meter<br>
  **nx, ny, nz**  is the normal vector of the trajectory in meter

## Limitations/Problems
* The normal and tangent vectors in the output file may flip. This is because they describe the trajectory and not the orientation of the melting probe. Currently there is no information about the melting probe orientation.
