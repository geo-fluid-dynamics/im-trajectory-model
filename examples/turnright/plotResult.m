function plotResult( varargin )

% read the result from IMTrajectoryModel (IMTM)
IMTMfilename='result.log';
IMTMdata = dlmread(IMTMfilename,' ',1,0);

inputData = importdata('turnright.csv');

% we want to start at the 2000th data point because the data at the
% beginning are not good
posToUse=2000;
inputDataStartPos=[inputData(posToUse,end-2)/1000,inputData(posToUse,end-1)/1000,inputData(posToUse,end)/1000];

% calc tangent vector
pos1=[inputData(posToUse,end-2)/1000,inputData(posToUse,end-1)/1000,inputData(posToUse,end)/1000];
pos2=[inputData(posToUse+100,end-2)/1000,inputData(posToUse+100,end-1)/1000,inputData(posToUse+100,end)/1000];
tangentVec=pos2-pos1;
tangentVec=tangentVec./norm(tangentVec);

% here we calculate the normal vector, which we later rotate a little
normalVec(1)=tangentVec(2);
normalVec(2)=tangentVec(1);
normalVec(3)=tangentVec(3);

% rotate the normal vector
normalVec = rodrigues_rot(normalVec,tangentVec,-pi/8);

% plot the trajectory from turnright.csv
h_inputData=plot3(inputData(:,end-2)/1000,inputData(:,end-1)/1000,inputData(:,end)/1000);
hold on;

% plot the position after which IMTM data exist
plot3(inputData(posToUse,end-2)/1000,inputData(posToUse,end-1)/1000,inputData(posToUse,end)/1000,'o');

% plot the normal vector
quiver3( inputData(posToUse,end-2)/1000,inputData(posToUse,end-1)/1000,inputData(posToUse,end)/1000,normalVec(1),normalVec(2),normalVec(3),0,'color',[0 0 0] ) 

% plot the tangent vector
quiver3( inputData(posToUse,end-2)/1000,inputData(posToUse,end-1)/1000,inputData(posToUse,end)/1000,tangentVec(1),tangentVec(2),tangentVec(3),0,'color',[1 0 0] ) 

% plot the result of IMTM
h_IMTMdata=plot3(IMTMdata(:,2)+inputDataStartPos(1),IMTMdata(:,3)+inputDataStartPos(2),IMTMdata(:,4)+inputDataStartPos(3),'color',[0 1 0]);

for i=1:500:length(IMTMdata(:,2))
    % plot arrows to display the torsion using n_fixed
    quiver3( IMTMdata(i,2)+inputDataStartPos(1),IMTMdata(i,3)+inputDataStartPos(2),IMTMdata(i,4)+inputDataStartPos(3),IMTMdata(i,12)/2,IMTMdata(i,13)/2,IMTMdata(i,14)/2,0,'color',[0 0 0] );
    
    % plot arrows to show n
    %quiver3( IMTMdata(i,2)+inputDataStartPos(1),IMTMdata(i,3)+inputDataStartPos(2),IMTMdata(i,4)+inputDataStartPos(3),IMTMdata(i,8),IMTMdata(i,9),IMTMdata(i,10),0,'color',[1 0 0] );
end

% change the orientation of axis
set(gca,'Zdir','Reverse');
set(gca,'Ydir','Reverse');

title("turnright example");

legend([h_inputData,h_IMTMdata],'Input data','IMTM data');

xlabel('x [m]');
ylabel('y [m]');
zlabel('z [m]');

axis equal;
hold off
grid on
set(findall(gca, 'Type', 'Line'),'LineWidth',2);
view(-95,3);

print -dpng result.png
end

