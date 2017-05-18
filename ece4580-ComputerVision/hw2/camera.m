%================================= camera ================================
%
%  Implements the camera projection equations to a CCD-type sensor.
%  Takes in the real world coordinates in meters of a point and returns 
%  the image coordinate location in pixels of the point.
%  
%
%  function icoords = camera(wcoords)
%
%  Input:
%    wcoords	-The world coordinates of a point in meters as a column
%		   vector.
%
%  Output:
%    icoords	-The image coordinates of the point in pixels as a 
%		   column vector.
%
%
%  If done properly, then passing a matrix of column vectors should perform
%  the projection equations for all of the associated vectors and return
%  a matrix of column vectors representing the projected 3D points.
%  This requires some savviness with Matlab (using .* and ./).
%
%================================= camera ================================

%
%  Name:	camera.m
%
%  Author:	Patricio A. Vela, pvela@ece.gatech.edu
%
%  Created:		2006/01/19
%  Modified:	2013/01/17
%
%================================= camera ================================

function icoords = camera(wcoords)

[M, N] = size(wcoords);

if (M ~= 3) 
  error('Input needs to be 3D column vector(s).');
end

xvals = wcoords(1,:);
yvals = wcoords(2,:);
zvals = wcoords(3,:);

f = .004; % focal length
sensorDim = [.0048, .0036];
sensorRes = [800, 600];

W = sensorRes(1);
H = sensorRes(2);
dr1 = sensorDim(1) / W;
dr2 = sensorDim(2) / H;

r1 = f * xvals ./ zvals;
r2 = f * yvals ./ zvals;

R1 = ceil(r1./dr1) + (W / 2);
R2 = ceil(r2./dr2) + (H / 2);

icoords = [R1; R2];

end
