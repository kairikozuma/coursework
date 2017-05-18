%================================ edgefind ===============================
%
%  edgeIm = edgefind(I, highT, lowT)
%
%  INPUTS:
%    I			- the image (should be double!)
%    highT		- the upper threshold.
%    lowT       - the lower threshold.
%
%  OUTPUTS:
%    escore		- the edge score.
%
%================================ edgefind ===============================

%
%  Name:		edgefind.m
%
%  Author:		Patricio A. Vela,			pvela@gatech.edu
%
%  Created:		2014/01/17
%  Modified:	2014/01/17
%
%================================ edgefind ===============================
function [detected] = edgefind(I, highT, lowT)


%-- Compute the gradient of the image.
cdx = [-1 0 1]/2;
cdy = cdx';

dIdx = imfilter(I, cdx, 'replicate');
dIdy = imfilter(I, cdy, 'replicate');

%-- Compute the edge score (sum of squares of derivatives).
score = dIdx.^2 + dIdy.^2;

%--Perform hysteresis with the upper and lower score thresholds.
highDetect = score > highT ; % Apply the upper threshold first.
[i, j] = find(highDetect);  % Using find, get indices of detected points.

lowDetect  = score > lowT;  % Apply the lower threshold second.

detected = bwselect(lowDetect, i, j);
                      % Using bwselect with the detected points on the
                      % low detection binary image, get more candidates.

detected = bwmorph(detected, 'thin', inf);
                      % Apply some thinning to get skinny edge lines.


end
