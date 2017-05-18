%=============================== playmatrix ==============================
%
%  function playmatrix(input)
%
%
%  Takes in a 3D matrix, which represents a time sequence of grayscale 
%  images, and plots each of the image slices with a short pause.
%  Will display in the current figure.
%
%
%  Input:
%    input	- The images seequence to display.
%    ptime	- [Optional] pause time between images.
%
%=============================== playmatrix ==============================

%
%  Name:	playmatrix.m
%
%  Author:	Patricio A. Vela, pvela@gatech.edu
%
%  Created:	2006/02/01
%  Modified:	2010/02/25
%
%=============================== playmatrix ==============================
function playmatrix(input, ptime)

if (nargin < 2)				% If no pause time given, set.
  ptime = 0.1;
end

[w h l] = size(input);			% Get image dimensions and #images.

for i=1:l
    colormap(gray)
    imagesc(input(:,:,i));
    axis off
    drawnow();
    pause(ptime);
end

end

%
%=============================== playmatrix ==============================
