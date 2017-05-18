%=============================== optflowHS ===============================
%
%  [Xu, Xv] = optflowHS(I1, I2, alphasq, iter)
%
%
%  Implements the Horn and Schunck optical flow algorithm for determination
%  of optical flow between two images.
%
%  Source:
%    Horn, BKP and Schunck, BG, â€œDetermining Optical Flow,â€? 
%      Artificial Intelligence, vol. 17, pp. 185-204, 1981.
%
%  Inputs:
%    I1		-the first image in time.
%    I2		-the second image in time.
%    alphasq	-the parameter alpha^2.
%    iter	-number of Gauss-Siedel iterations.
%
%=============================== optflowHS ===============================
function [Xu, Xv] = optflowHS(I1, I2, alphasq, iter)

Xu = zeros(size(I1));			% This is the initial guess.
Xv = zeros(size(I2));

[Ix, Iy, It] = differentialsHS(I1, I2);

% Missing code here for the constant parts.
k = 1;
denom = (k * alphasq + Ix.^2 + Iy.^2);

for i=1:iter
  barXu = lapAverage(Xu);
  barXv = lapAverage(Xv);

  % Missing code here for iterative solver.
  gradConst = (Ix .* barXu + Iy .* barXv + It);
  Xu = barXu - (gradConst .* Ix)./ denom;
  Xv = barXv - (gradConst .* Iy)./ denom;
end

end

%--------------------------- differentialsHS ---------------------------
%
% [Ix, Iy, It] = differentialsHS
%
%--------------------------- differentialsHS ---------------------------
function [Ix, Iy, It] = differentialsHS(I1, I2)

% Missing code here.  Make sure to take care of boundary conditions.

Kt = (1.0/4.0)*[1,1;1,1];
Kx = (1.0/4.0)*[-1,1;-1,1];
Ky = (1.0/4.0)*[-1,-1;1,1];

It = imfilter(I2, Kt) - imfilter(I1, Kt);
Ix = imfilter(I1, Kx) + imfilter(I2, Kx);
Iy = imfilter(I1, Ky) + imfilter(I2, Ky);


end
%------------------------------ lapAverage -----------------------------
%
% [ubar] = lapAverage(u)
%
%------------------------------ lapAverage -----------------------------
function [ubar] = lapAverage(u)

% Missing code here.  Make sure to take care of boundary conditions.

k = 3;
A = (1/12.0)*[1,2,1;2,0,2;1,2,1];
%lapU = k * u + imfilter(u, k * A);
%ubar = mean(mean(lapU));
ubar = imfilter(u, A);
end
