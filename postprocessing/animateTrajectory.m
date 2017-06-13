function [ output_args ] = animateTrajectory( imTrajectoryFilename )
%ANIMATETRAJECTORY Summary of this function goes here
%   Detailed explanation goes here

imTrajectoryFilename='../IMtrajectoryModel/trajectory.log';

addpath('../submodules/VisIM/classes');
addpath('../submodules/VisIM/functions');

% create a simple melting probe with the shape of a rectangular box 
IM=createIM(2,0.15,0.05);

data = dlmread(imTrajectoryFilename,' ',1,0);

for i=1:length(data(:,1))
    if mod(i,50) && i~=1
        continue
    end
    
    % calculate the euler angles from the rotation between the trangential
    % vectors
    r = vrrotvec([data(1,5) data(1,6) data(1,7)],[data(i,5) data(i,6) data(i,7)]);
    rotm = vrrotvec2mat(r);
    eul = rotm2eul(rotm);
    
    IM.transformCoordinates([data(i,2) data(i,3) data(i,4)],[eul(1) eul(2) eul(3)]);
    if i==1
        h_axis=axes('Units','normalized');
        axis equal;
        view(30, 45);
        
        IM.volumeMode=false;
        IM.setStartPos;
    end
    
    % store the current view position
    viewSave=get(h_axis,'view');
    
    % clear everything in the axis
    cla(h_axis,'reset');
    %axes(h_axis);
    
    IM.plotAll(h_axis);
    
    % plot the curve of the melting head
    hold on;
    plot3(data(1:i,2),data(1:i,3),data(1:i,4));
    
    % plot frenet frame
    quiver3(data(i,2),data(i,3),data(i,4),data(i,5),data(i,6),data(i,7),0,'color',[1 0 0]);
    quiver3(data(i,2),data(i,3),data(i,4),data(i,8),data(i,9),data(i,10),0,'color',[0 0 1]);
    
    % set to the old view position
    set(h_axis,'view',viewSave);
    
    axis equal
    xlim([-.5 .5]);
    ylim([-3 3]);
    zlim([-8 2.5]);
    title(num2str(i));
    pause(.01);
    drawnow
end

end

