% Creates a decaying inusoid

function xs = myDecayingSinusoi(A, b, omega, phi, fs, tStart, tEnd)
% A = initial amplitude
% b = decay constant
% omega = frequency in radians per second
% phi = phase, time offset of first peak
% fs = number of sample values per second
% tStart = starting time in sec
% tEnd = ending time in sec
tt = tStart:1/fs:tEnd; % time indices
xs = A*exp(-b*tt).*cos(omega*tt+phi);
end