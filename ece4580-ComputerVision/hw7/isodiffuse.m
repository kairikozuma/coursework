%=============================== isodiffuse ==============================
%
%  Performs isotropic diffusion by running the heat equation on a grayscale
%  image.
%
%  function sI = isodiffuse(gI, dt, iter)
%
%
%  Inputs:
%    gI		- grayscale/scalar image (should be floating point format).
%    dt		- timestep to use in running the heat equation 
%		      the argument should be 0 < dt < 0.5.
%    iter	- total number of iterations.
%
%  Output:
%    sI      - the smoothed image.
%
%=============================== isodiffuse ==============================

%
%  Name:		isodiffuse.m
%
%  Author:		Patricio A. Vela, pvela@gatech.edu
%
%  Created:		2010/01/05
%  Modified:	2010/01/05
%
%=============================== isodiffuse ==============================
function sI = isodiffuse(gI, dt, iter)

for i=1:iter
    dI = del2(gI);
    gI = gI - dI * dt;
end

sI = gI;


end
%
%=============================== isodiffuse ==============================
