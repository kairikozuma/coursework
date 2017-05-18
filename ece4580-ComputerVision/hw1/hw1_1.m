%==========================
%  Name:		hw1_1.m
%
%  Author:		Kairi Kozuma
%
%==========================
A = [-7,6,-2,-9;-14,-5,-4,0;-12,14,8,-2;10,-14,9,4];
[UU, SS, VV] = svd(A);
fprintf('UU\n');
disp(UU);
fprintf('SS\n');
disp(SS);
fprintf('VV\n');
disp(VV);

fprintf('Singular Values of SS\n');
disp([SS(1,1),SS(2,2),SS(3,3),SS(4,4)]);
fprintf('Last column of VV\n');
disp([VV(1,4),VV(2,4),VV(3,4),VV(4,4)]');