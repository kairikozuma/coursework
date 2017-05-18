%==========================
%  Name:		hw3_3.m
%
%  Author:		Kairi Kozuma
%
%==========================

% Transformation matrix
gWC = [4.32978028117747e-17,0.707106781186548,0.707106781186548,4;-0.866025403784439,0.353553390593274,-0.353553390593274,3;-0.500000000000000,-0.612372435695795,0.612372435695795,2;0,0,0,1];
p1_C = [24;-9.50000000000000;25];
p2_W = [12.8926822012171;-78.3963768496259;10.5141258557173];
psi = [250,0,240;0,250,320;0,0,1];
r3 = [251;57];
 
% a) Give the ray (in camera units/pixels) that the point 
%    pC1 = (24.0000, ?9.5000, 25.0000)T lies on. 
%    Also give it as a ray with the last element normalized 
%    (this would be the actual pixel location in the image that the point 
%    projects to, modulo the quantization).

    ray1 = psi * p1_C;
    ray1normalized = ray1(1:2) / ray1(3);

    fprintf('a1) Ray 1:\n');
    disp(ray1);
    
    fprintf('a2) Ray 1 normalized:\n');
    disp(ray1normalized);
        
% b) Give the ray (in camera units/pixels) that the point 
%    pW2 = (12.8927, ?78.3964, 10.5141)T lies on. 
%    Please take care that this point is in world coordinates.
    
    R_WC = gWC(1:3,1:3); 
    T_WC = gWC(1:3,4);
    gCW = [R_WC',-R_WC'*T_WC;0,0,0,1];
    gCWp2 = (gCW * [p2_W;1]);
    ray2 = psi * gCWp2(1:3,:);
    
    fprintf('b1) Ray 2:\n');
    disp(ceil(ray2));
    
    ray2normalized = ray2(1:2) / ray2(3);
    
    fprintf('b2) Ray 2 normalized:\n');
    disp(ceil(ray2normalized));
    
% c) Give the ray (in world units) that the image point 
%    ?r3 = (251.0000, 57.0000)T maps to when back-projecting the pixel 
%    back out into the world. Back-projection is the partial inversion 
%    process discussed in class that maps the 2D image point back out
%    to a 3D ray. Let the ray be given in camera frame coordinates. 
    
    q3_W = gWC * [inv(psi) * [r3;1];1]; 
    p3_W = q3_W(1:3,:);
    
    fprintf('c) Back projection of ray 3:\n');
    disp(p3_W);
    
    % Check that it projects to same ray
    gCWp3 = (gCW * [p3_W;1]);
    r3check = psi * gCWp3(1:3,:) / gCWp3(3,1);
    