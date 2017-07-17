# IMtrajectoryModel
A tool that converts heater state changes of the IceMole to a trajectory.

The heater state changes can be provided by two possible input files, namely a logfile and a simplified logfile. An example for a logfile is given by `IMtrajectoryModel/test/logfile/icemole_state.log`. Examples for simplified logfiles can be found in `IMtrajectoryModel/test/simplifiedLogfile/`.

Author: K. Schüller schueller@aices.rwth-aachen.de

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
The IDs of the heating cartridges and wall heaters are used to define the melting probe COS.

### Ice COS (global COS)

## Melting Probe Orientation
The orientation of the melting probe can be set with the initial tangent `t_0` and the initial normal vector `n_0`, which defines the direction of the  melting head heating cartridges 1, 2, 3, 4, 5, 6, 7, 8

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
