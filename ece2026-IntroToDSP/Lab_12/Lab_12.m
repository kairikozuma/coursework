%% 3.1
b = [1, -1];
w = -pi:1/1000:pi;
[hh, ww] = freqz(b, 1, w);
plot(ww, abs(hh));
title('First Difference Frequency Response');
ylabel('Magnitude');
xlabel('Normalized Frequency');


[xx, fs, n] = wavread('catsdogs.wav');
yy = filter(b,1, xx);
tt = 0:1/fs:length(xx)/fs;

figure
subplot(2,1,1)
plotspec(xx,fs);
title('CatsandDogs.wav');
ylabel('Magnitude');
xlabel('Time (sec)');
subplot(2,1,2)
plotspec(yy,fs);
title('CatsandDogs.wav First Difference');
ylabel('Magnitude');
xlabel('Time (sec)');

soundsc(xx, fs);
soundsc(yy, fs);

%% 3.2
figure 
fs = 8000;
t = 0:1/fs:1.3;;
b1 = (1.1 + cos(2*pi*81*t));
b2 = cos(2*pi*1031*t);
bb = b1.*b2;
subplot(2,1,1)
plot(t,bb);
title('AM Modulation Signal');
ylabel('Magnitude');
xlabel('Time (sec)');
subplot(2,1,2);
plot(t,abs(bb));
title('Magnitude of AM Modulation Signal');
ylabel('Magnitude');
xlabel('Time (sec)');

figure
subplot(3,1,1)
plotspec(bb, fs);
title('Spectrogram of AM Signal');
ylabel('Frequency');
xlabel('Time (sec)');
subplot(3,1,2)
plotspec(b1, fs);
title('Spectrogram of Envelope');
ylabel('Frequency');
xlabel('Time (sec)');
subplot(3,1,3)
plotspec(abs(bb), fs);
title('Spectrogram of AM Signal Magnitude');
ylabel('Frequency');
xlabel('Time (sec)');

%% 3.2b
bb3 = abs(bb);
load filter.mat
bb3y = filter(num, den, bb3);
figure
subplot(2,1,1)
plot(t,bb3y);
title('AM Signal Magnitude Cubed through LPF');
ylabel('Magnitude');
xlabel('Time (sec)');
subplot(2,1,2);
plot(t,b1);
title('Modulation');
ylabel('Magnitude');
xlabel('Time (sec)');

