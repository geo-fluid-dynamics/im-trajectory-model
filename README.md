# IMtrajectoryModel
A tool that converts heater states from an IceMole logfile to a trajectory.

Currently only the heater states (24 on-off states) and the timestamps are used. An example of such a logfile is given in `IMtrajectoryModel/test/logfile/icemole_state.log`.

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
In order to use IMtrajectoryModel to calculate a melting trajectory from an IceMole logfile you can run
```
./IMtrajectoryModel -l LOGFILE -i INPUTFILE -o OUTPUTFILE
```
in which `LOGFILE` is the location of the IceMole logfile, `INPUTFILE` the location of the input file (e.g. inputs.ini) and `OUTPUTFILE` the location where you want to save the melting trajectory.

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
