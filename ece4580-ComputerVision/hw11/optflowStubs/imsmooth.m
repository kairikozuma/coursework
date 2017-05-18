%================================= smooth ================================
%
%  J = imsmooth(I, ...)
%
%  Smooth an image using a Gaussian convolution kernel.
%
%  Inputs:
%    I		- The RGB image to unsharp.
%    ...	- WHAT OTHER PARAMETERS DO YOU NEED FOR YOU ALGORITHM?
%
%================================= smooth ================================
function J = imsmooth(I, sigma)

J = imgaussfilt(I, sigma);
end
