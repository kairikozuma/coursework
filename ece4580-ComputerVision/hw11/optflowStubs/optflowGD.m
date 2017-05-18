%=============================== optflowGD ===============================
%
%  [Xu, Xv] = optflowGD(I1, I2, alphasq, iter, dt)
%
%
%  Implements the gradient descent optical flow algorithm based upon
%  the energy functional of Horn and Schunck.  Computes the optical flow
%  between two images.
%
%  Inputs:
%    I1		-the first image in time.
%    I2		-the second image in time.
%    alphasq	-the parameter alpha^2.
%    iter	-number of gradient descent iterations.
%    dt		-the timestep to use in gradient descent.
%
%=============================== optflowGD ===============================
function [Xu, Xv] = optflowGD(I1, I2, alphasq, iter, dt)

Xu = zeros(size(I1));			% This is our initial guess.
Xv = zeros(size(I2));

[Ix, Iy, It] = differentialsGD(I1, I2);

for i=1:iter
  lapXu = laplacian(Xu);
  lapXv = laplacian(Xv);

  grad = Ix .* Xu + Iy .* Xv + It;
  
  Xu = Xu - dt*(grad .* Ix - alphasq * lapXu);
  Xv = Xv - dt*(grad .* Iy - alphasq * lapXv);
end


end
%--------------------------- differentialsGD ---------------------------
%
% [Ix, Iy, It] = differentialsGD
%
%--------------------------- differentialsGD ---------------------------
function [Ix, Iy, It] = differentialsGD(I1, I2)

Kt = (1.0/4.0)*[1,1;1,1];
Kx = (1.0/4.0)*[-1,1;-1,1];
Ky = (1.0/4.0)*[-1,-1;1,1];

It = imfilter(I2, Kt) - imfilter(I1, Kt);
Ix = imfilter(I1, Kx) + imfilter(I2, Kx);
Iy = imfilter(I1, Ky) + imfilter(I2, Ky);

%TODO: Boundary conditions
end

%------------------------------ laplacian ------------------------------
%
% [ubar] = laplacian(u)
%
%------------------------------ laplacian ------------------------------
function [lapu] = laplacian(u)
%TODO: Boundary conditions
k = 3;
A = (1/12.0)*[1,2,1;2,0,2;1,2,1];
lapu = k * u + imfilter(u, k * A);
end
