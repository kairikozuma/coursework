%% Lab_05_Prelab
%% Part 2.2
fSamp = 8000;   %-Number of time samples per second
dt = 1/fSamp;
tStart = 0;
tStop = 1.5;
mu = -1000;
f = 3500;
tt = tStart:dt:tStop;
psi = 2*pi*(mu*tt.*tt+f*tt);
cc = real( 7.7*exp(j*psi) );
soundsc( cc, fSamp );
fzero = 400;
phi = 2*pi*rand;
plotspec( cc+j*1e-12, fSamp, 256 ), colorbar, grid on   %-- with negative frequencies

% Time
fprintf('\ntt vector has size %d with duration %f', length(tt), length(tt)/fSamp);
maxf = max(f, (2 * mu * tStop) + f);
minf = min(f, (2 * mu * tStop) + f);
fprintf('\nMax Frequency: %0.0f\nMin Frequency: %0.0f\n', maxf, minf);

%% Part 2.3
amps = [0.1, 1, 2, 4, 10, 100];
dBfamps = num2dB(amps);
dB = [-6, -60, -80];
ampfdB = dB2num(dB);

% The frequency resolution of the spectrogram is inversely proportional to
% the Section Length. In other words, when the true spectrum has two lines
% (at f1 and f2) these two lines will be visible as distinct lines in the
% spectrogram if |f1 ?f2| ? C/TSECT where C is a proportionality constant
% and TSECT is the section duration in secs.


%% Part 2.4
fd = 4;
fc = 1024;
te = 5;
fs = 8000;
tt = 0:1/fs:te;
b = 10*cos(2*pi*fd*tt+rand(1)).*cos(2*pi*fc*tt+rand());
subplot(1,6,1);
plotspec(b,fs,256); colorbar, grid on, zoom on
subplot(1,6,2);
plotspec(b,fs,512); colorbar, grid on, zoom on
subplot(1,6,3);
plotspec(b,fs,1024); colorbar, grid on, zoom on
subplot(1,6,4);
plotspec(b,fs,2048); colorbar, grid on, zoom on
subplot(1,6,5);
plotspec(b,fs,4096); colorbar, grid on, zoom on
subplot(1,6,6);
plotspec(b,fs,8192); colorbar, grid on, zoom on
