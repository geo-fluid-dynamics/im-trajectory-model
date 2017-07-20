function plotResult( varargin )

% read the result from IMTrajectoryModel (IMTM)
IMTMfilename='result.log';
IMTMdata = dlmread(IMTMfilename,' ',1,0);

inputData = importdata('turnright.csv');

% we want to start at the 2000th data point
posToUse=2000;
inputDataStartPos=[inputData(posToUse,end-2)/1000,inputData(posToUse,end-1)/1000,-inputData(posToUse,end)/1000];

normalVec=[0.0883,-0.9812,0.1717];

% plot the trajectory from turnright.csv
h_inputData=plot3(inputData(:,end-2)/1000,inputData(:,end-1)/1000,-inputData(:,end)/1000);
hold on;

% plot the position after which IMTM data exist
plot3(inputData(posToUse,end-2)/1000,inputData(posToUse,end-1)/1000,-inputData(posToUse,end)/1000,'o');

% plot the normal vector
quiver3( inputData(posToUse,end-2)/1000,inputData(posToUse,end-1)/1000,-inputData(posToUse,end)/1000,normalVec(1),normalVec(2),normalVec(3),0,'color',[0 0 0] ) 

% plot the result of IMTM
h_IMTMdata=plot3(IMTMdata(:,2)+inputDataStartPos(1),IMTMdata(:,3)+inputDataStartPos(2),IMTMdata(:,4)+inputDataStartPos(3),'color',[1 0 0]);

title("turnright example");

legend([h_inputData,h_IMTMdata],'Input data','IMTM data');

xlabel('x [m]');
ylabel('y [m]');
zlabel('z [m]');

axis equal;
hold off
grid on

print -dpng result.png
end

