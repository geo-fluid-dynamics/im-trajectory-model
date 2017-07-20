function plotTrajectory( imTrajectoryFilename )

imTrajectoryFilename='../build/trajectory.log';

data = dlmread(imTrajectoryFilename,' ',1,0);

plot3(data(:,2),data(:,3),data(:,4),'color',[1 0 0])

xlabel('x [m]');
ylabel('y [m]');
zlabel('z [m]');

axis equal;
hold off
end

