function plotResult( varargin )

addpath('../../postprocessing/functions')

% read the result from IMTrajectoryModel (IMTM)
IMTMfilename='result.log';
IMTMdata = dlmread(IMTMfilename,' ',1,0);

% plot the result of IMTM
plotColorLine3d(IMTMdata(2:end,2),IMTMdata(2:end,3),IMTMdata(2:end,4),IMTMdata(2:end,15),'linewidth',5);
hold on

% change the orientation of axis
set(gca,'Zdir','Reverse');
set(gca,'Ydir','Reverse');

title("driveU example");

%%legend([h_inputData,h_IMTMdata],'Input data','IMTM data');

xlabel('x [m]');
ylabel('y [m]');
zlabel('z [m]');

colorbar;
colormap jet;

hold off
grid on
set(findall(gca, 'Type', 'Line'),'LineWidth',2);
view(45,20);

print -dpng result.png
end

