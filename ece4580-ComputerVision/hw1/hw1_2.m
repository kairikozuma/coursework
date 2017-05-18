%==========================
%  Name:		hw1_2.m
%
%  Author:		Kairi Kozuma
%==========================
q1 = [-36.6; -25.7;  66.0];
q2 = [ 23.1;   0.1;  77.0];
q3 = [-45.0;  49.7; 150.0];
q4 = [-81.0;  89.5; 270.0];

p1 = pcamera(q1);
p2 = pcamera(q2);
p3 = pcamera(q3);
p4 = pcamera(q4);

fprintf('Q1 projects to:\n');
disp(p1);
fprintf('Q2 projects to:\n');
disp(p2);
fprintf('Q3 projects to:\n');
disp(p3);
fprintf('Q4 projects to:\n');
disp(p4);

fprintf('All points project onto image plane if the range is [-0.5,0.5]\n');
fprintf('There are four unique projections. Q3 and Q4 project almost onto the same point.\n');