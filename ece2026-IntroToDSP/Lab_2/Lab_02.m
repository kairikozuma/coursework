%%%%% Part 1 %%%%
% Make Plot of sum of cosines
dt = 1/800;
XX = rand(1,3).*exp(2*j*pi*rand(1,3));3
freq = 9;
ccsum = zeros(3,500);
tk = 1:500;
t = tk*dt;
    
% Create amplitude and phase vectors
% Both have length of XX
A = abs(XX);
phi = angle(XX);

% Create mesh grid of k values and phi values
% meshes have size of phi x size of k
[tkmesh, phimesh] = meshgrid(tk, phi);

% Create time mesh
tmesh = tkmesh*dt;

% Use dot product to sum sinusoids
ccsum = A*cos(2*pi*freq*tmesh + phimesh);

plot(t, ccsum);
title('Sum of Sinusoids');
xlabel('Time (seconds)');
ylabel('Magnitude');
grid on, zoom  on, shg

%%%% Part 2 %%%%
mySig.freq = 2;    %-- (in hertz)
mySig.complexAmp = 5*exp(j*pi/4);
dur = 2;
start = -1;
dt = 1/(32*mySig.freq);
mySigWithVals = makeCosVals(mySig, dur, start, dt);
figure
plot(mySigWithVals.times, mySigWithVals.values);
title('Make Cosine Function');
xlabel('Time (seconds)');
ylabel('Magnitude');
grid on, zoom  on, shg

%%%% Part 3 %%%%
ss(1).freq = 15; ss(1).complexAmp = exp(j*pi/4);
ss(2).freq = 12; ss(2).complexAmp = 2i;
ss(3).freq = 9; ss(3).complexAmp = -4;

dur = 1;
tstart = -0.5;
dt = 1/(15*32);

ssOut = addCosVals(ss, dur, tstart, dt);
figure
plot(ssOut.times, ssOut.values);