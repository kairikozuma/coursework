%==========================
%  Name:		hw3_5.m
%
%  Author:		Kairi Kozuma
%
%==========================

fprintf('a)Inverse matrix result\n');

x1 = [4.90000000000000;0.200000000000000];
y1 = [66;54.660000000000004];
x2 = [-2.600000000000000;1.800000000000000];
y2 = [-41.120000000000000;-21.760000000000000];

yvec = [y1;y2];
xmat = [x1',0,0;0,0,x1';x2',0,0;0,0,x2'];
avec = inv(xmat)*yvec;
solutionA = reshape(avec,2,2)';
disp(solutionA);

fprintf('b)Singular Value Decomposition result\n');

mat = [xmat, -yvec;0,0,0,0,0];
[UU, SS, VV] = svd(mat);
avecSVD = VV(:,5)./VV(5,5);
avecSVD = avecSVD(1:4);
solutionB = reshape(avecSVD,2,2)';
disp(solutionB);