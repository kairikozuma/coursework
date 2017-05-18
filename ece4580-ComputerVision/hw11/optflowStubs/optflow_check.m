%============================= optflow_check =============================
%
%  function wI = optflow_check(I1, I2, Xof, method)
%
%
%  Takes in two images and their computed optical flow field and compares
%  the second image against the image predicted by warping the first image
%  according to optical flow.  If all assumptions associated to the optical
%  flow solver hold for the image pair, then the two images should be pretty
%  close.
%
%
%  Inputs:
%    I1, I2		- Inputs images to optical flow algorithm.
%    Xof		- Computed optical flow, as a structure.
%			    Xof.u and Xof.v are horizontal and vertical.
%    method		- Method for computing warp (optional arg.).
%			  'forward', 'backsteps', 'backward' 
%			  [Default: 'backward']
%
%  Outputs:
%    wI			- If desired, warped output is returned.
%
%  Outputs:		N/A.
%
%============================= optflow_check =============================

%
%  Name:	optflow_check.m
%
%  Author:	Patricio A. Vela, pvela@gatech.edu
%
%  Created:	2007/05/02
%  Modified:	2010/02/25
%
%============================= optflow_check =============================
function intI = optflow_check(I1, I2, X, method)

if (~isfloat(I1) || ~isfloat(I2))	% check if double.
  I1 = double(I1);			% if not, then convert to double.
  I2 = double(I2);
end

if (nargin < 4)				% verification method.
  method = 'backward';
end

[M, N] = size(I1);			% rows, cols.
[n.x, n.y] = meshgrid([1:N],[1:M]);	% cols, rows.

switch method
  case 'backward',			% simplest approach, just look
    n.x = n.x - X.u;			% back from where things came
    n.y = n.y - X.v;			% to generate new image.

    intI = interp2(I1, n.x, n.y);

  case 'backstep'			% take baby steps using backward.
    intI = I1;
    for i=1:15
      n.x = n.x - (1/250)*X.u;
      n.y = n.y - (1/250)*X.v;

      intI = interp2(intI, n.x, n.y);
    end

  case 'forward',			% move image forward and assign.
    f.x = n.x + X.u;
    f.y = n.y + X.v;

    intI = griddata(f.x, f.y, I1, n.x, n.y);
end

% figure(9);
%   clf;
%   imagesc(I1);
%   axis image;
%   colormap('gray');
%   title('I1');
% 
% figure(10);
%   clf;
%   imagesc(I2);
%   axis image;
%   colormap('gray');
%   title('I2');
% 
% figure(11);
%   clf;
%   imagesc(intI);
%   axis image;
%   colormap('gray');
%   title('Warped I1');
% 
% figure(12);
%   imagesc(I2 - intI);
%   colorbar;
%   title('Error.');
