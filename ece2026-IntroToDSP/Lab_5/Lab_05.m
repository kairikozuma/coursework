%% 3.1
fs = 4000;
tt = 0:1/fs:5;
slope = (11400 - 900)/5;
chirp = cos(2*pi*(900*tt + (slope/2)*tt.*tt));

chirpsec = chirp(2*fs:2.5*fs);
tsec = length(chirpsec)/fs;
ls = tsec * fs;
plotspec(chirp, fs, 64);
ylim([-1000,3000])
fprintf('\nSection Length Duration is %f\n', ls/fs);

%% 3.2
T = 1;
amp = 1;
tStop= 5;
fs = 1000;
tt = 0:1/fs:tStop;
xx = amp*abs(sin(2*pi*tt/T));
figure
plot(tt, xx);
figure
tsec = 2.5;
lsec = tsec*fs;
fprintf('Lsec is %f\n', lsec);
plotspec(xx, fs, lsec);
ylim([0,15])
%% 3.3
fprintf('Double the amplitude is %1.2fdB greater\n\n', num2dB(2));

%% 3.4
% figure
% subplot(1,3,2);
% k1 = plotspecDB(xx, fs, 2048, 80);
% title('T = 1');
% ylim([0,200]);

T2 = 2;
xx = amp*abs(sin(2*pi*tt/T2));
figure
plot(tt, xx)
% subplot(1,3,3);
figure
k2 = plotspecDB(xx, fs, 5000, 80);
title('T = 2');
ylim([0,500]);

% T5 = .5;
% xx = amp*abs(sin(2*pi*tt/T5));
% subplot(1,3,1);
% k5 = plotspecDB(xx, fs, 4096, 80);
% title('T = .5');
% ylim([0,200]);