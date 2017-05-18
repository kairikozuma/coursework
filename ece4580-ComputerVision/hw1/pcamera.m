%================================= camera ================================
%
%  Implements the pinhole projection equations of the world to an image
%  plane.  Takes in the real world coordinates in meters of a point and
%  returns the image coordinate location in meters.
%  
%  function ipcoords = camera(wcoords)
%
%  Input:
%    wcoords	-The world coordinates of a point in meters as a column
%		   vector.
%
%  Output:
%    icoords	-The image plane coordinates of the point in meters as a 
%		   column vector.
%
%
%  If done properly, then passing a matrix of column vectors should perform
%  the projection equations for all of the associated vectors and return
%  a matrix of column vectors representing the projected 3D points.
%  This requires some savviness with Matlab (using ./).
%
%================================= camera ================================

%
%  Name:	    pcamera.m
%
%  Author:	    Patricio A. Vela, pvela@ece.gatech.edu
%
%  Created:	    2014/08/20
%  Modified:	2014/08/20
%
%================================= camera ================================
function ipcoords = pcamera(wcoords)

f = 6/10;

%--1 Check input dimensions.
[M, N] = size(wcoords);
if (M ~= 3) 
  error('Input needs to be 3D column vector(s).');
end

%--2 Apply pinhole projection equations.
ipcoords = (f / wcoords(3)) * [wcoords(1),wcoords(2)];

end
