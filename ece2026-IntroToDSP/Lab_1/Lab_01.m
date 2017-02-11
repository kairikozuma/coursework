% Lab 1 1/15/15

% % Reverse input string
% str = input('Input Text: ','s');
% strinv = str(end:-1:1);
% fprintf('\n%s\n', strinv);

% Plot decaying sinusoid
a = 3.817;
b = .6;
om = 250*2*pi;
ph = pi/4;
fs = 100;
ts = 0;
te = 2;
xDecay = myDecayingSinusoi(a, b, om, ph, fs, ts, te);

time1 = ts:1/fs:te;
plot(time1,xDecay), grid on
title('Decaying Sinusoid');
xlabel('Time (s)');

% Import and plot wav file
[wav, sr, bits] = wavread('ece2026Lab01.wav');
tsta = .25;
tend = .5;
duration = length(wav)/sr;
time2 = linspace(0,duration,length(wav));
time2sec = .25:1/sr:.5;
section = wav(length(wav)*(tsta/duration):length(wav)*(tend/duration));
title('Wave File Section ece2026Lab01.wav');
plot(time2sec, section);
xlabel('Time (s)');

% Play audible sound from data
soundsc(wav, sr);

% Original Wav Plot
figure;
plot(time2, wav);
title('Wave File ece2026Lab01.wav');
xlabel('Time (s)');

% Reverse Time Axis and Plot It
wavrev = wav(end:-1:1);
figure
plot(time2, wavrev);
title('Wave File ece2026Lab01.wav reversed');
xlabel('Time (s)');

% Write out reversed wave file
wavwrite(wavrev, sr, 'ece2026Lab01Rev.wav');



