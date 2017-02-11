% Lab 3
%%%% Part 1 %%%%
% Construct Beat signal xx = cos(2*pi*560*t) + cos(2*pi*580*t);
sigBeat = struct('Amp', 1, 'fc', 570, 'phic', 0, 'fDelt', 10, 'phiDelt', 0, 't1', 0, 't2', 4, 'fs', 8000);
sigBeatSum = sum2BeatStruct(sigBeat);
spectrogram(sigBeatSum.values, 1024, [], [], 8000, 'yaxis');
colorbar;
figure;
plot(sigBeatSum.times, sigBeatSum.values);
soundsc(sigBeatSum.values);

%%%% Part 2 %%%%
fs = 8000;
a = 1;
p = 0;
kd = .4;
bl = zeros(1,fs*.1);

% Start from c4 and go up one scale
c4 = key2sinus(40,a,p,fs,kd);
d4 = key2sinus(42,a,p,fs,kd);
e4 = key2sinus(44,a,p,fs,kd);
f4 = key2sinus(45,a,p,fs,kd);
g4 = key2sinus(47,a,p,fs,kd);
a5 = key2sinus(49,a,p,fs,kd);
b5 = key2sinus(51,a,p,fs,kd);
c5 = key2sinus(52,a,p,fs,kd);
scale = [bl c4 bl d4 bl e4 bl f4 bl g4 bl a5 bl b5 bl c5 bl];
figure;
spectrogram(scale, 1024, [], [], fs, 'yaxis');
colorbar;
figure;
t = linspace(1,length(scale)/8000,length(scale));
plot(t,scale);
soundsc(scale);

%%%% Part 3 %%%%
freq1 = 770;
freq2 = 1336
Ts = 0.3e-3;    % Sampling period
fsamp = 1/Ts;   % Sampling rate
tt = 0:1/fsamp:0.3;
DTMFsig = cos(2*pi*freq1*tt + rand(1)) + cos(2*pi*freq2*tt + rand(1)); % random phases
xx = zeros(1, round(2/Ts)); % Preallocate vector to hold DTMF signals
n1 = round(0.6/Ts);
n2 = n1 + length(DTMFsig)-1;
xx(n1:n2) = xx(n1:n2) +DTMFsig;
soundsc(xx,fsamp);
figure;
plotspec(xx, fsamp);
figure;
spectrogram(xx, 1024, [], [], fsamp, 'yaxis');
grid on;