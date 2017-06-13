function [ output_args ] = plotTrajectory( imTrajectoryFilename )
%PLOTTRAJECTORY Summary of this function goes here
%   Detailed explanation goes here

imTrajectoryFilename='../IMtrajectoryModel/trajectory.log';

addpath('../submodules/VisIM/classes');
addpath('../submodules/VisIM/functions');

data = dlmread(imTrajectoryFilename,' ',1,0);

plot3(data(:,2),data(:,3),data(:,4));

xlabel('x [m]');
ylabel('y [m]');
zlabel('z [m]');

%axis equal;

end

