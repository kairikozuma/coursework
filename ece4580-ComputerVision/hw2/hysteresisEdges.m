%============================ hysteresisEdges ============================
%
%  script hysteresisEdges.m
%
%
%  Loads the edgethresh.mat Matlab file (make sure to have it in your 
%  path or your current directory) and then applies hystersis-based
%  edge finding to identify which parts of the image reflect edge-like 
%  structures.
%
%============================ hysteresisEdges ============================

%
%  Name:		hysteresisEdges.m
%
%  Author:		Patricio A. Vela,			pvela@gatech.edu
%
%  Created:		2014/01/18
%  Modified:	2014/01/18
%
%============================ hysteresisEdges ============================

%--[1] Load the edgethresh Matlab file. 
load('edgethresh.mat');
I = double(I);          % Convert to double or crazy stuff happens.

highT = 145;
lowT = 120;

fprintf('High threshold: %d\nLow threshold: %d\n', highT, lowT);

%--[2] Run the edge finding function to get a binary image.
detect = edgefind(I, highT, lowT);

%--[4] Plot the image and also visualize the detected edge locations.
figure(1);
  imagesc(I);
  colormap('gray');
  title('Original image');
  axis image;
  
figure(2);
  imagesc(detect);
  colormap('gray');
  title('Edge detected image');

% c) 
% 
% The results are better than using a single
% threshold, since two thresholds captures more edges that have lower
% contrast with its environment. For example, the image obtained from
% two thresholds shows more of edges in the trees.
