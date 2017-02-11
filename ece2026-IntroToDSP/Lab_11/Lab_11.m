%% Lab_11
%% 3.1
zer = [(1/0.9)*exp(j*pi/3), (1/0.9)*exp(-j*pi/3)];
pol= [0.9*exp(j*pi/3), 0.9*exp(-j*pi/3)];
Bz = poly(zer);
Az = poly(pol);

%% 3.2
zer = [exp(j*pi/4), exp(-j*pi/4)];
pol = [0.95*exp(j*pi/4), 0.95*exp(-j*pi/4)];
Bz = poly(zer);
Az = poly(pol);

%% 3.3
hn = filter([1,2],[1,-0.9],[1,zeros(1,50)]);
n = 1:length(hn);
stem(n-1, hn);

%% 3.4
[ECG, fs, fint] = ECGmakeSig15('kkozuma3', 10);
phi = 2*pi*fint/fs;

zer = [exp(j*phi), exp(-j*phi)];
pol = [0.95*exp(j*phi), 0.95*exp(-j*phi)];
Bz = poly(zer);
Az = poly(pol);

ww = -pi:1/1000:pi;
[HH, WW] = freqz(Bz,Az,ww);
f = ww*fs/(2*pi);
plot(f,abs(HH));

ECGpart = ECG(1:5000)';
tt1 = 0:(1/fs):(floor(length(ECGpart)-1))/fs;

figure
subplot(2,1,1)
plot(tt1,ECGpart)
title('Original Signal');
xlabel('Time');
ylabel('Magnitude');
subplot(2,1,2)
yy= filter(Bz,[-Az(2), -Az(3)],ECGpart);
tt2 = 0:(1/fs):(floor(length(yy)-1))/fs;
plot(tt2,yy);
title('Interference Removed Signal');
xlabel('Time');
ylabel('Magnitude');

figure
subplot(2,1,1)
plotspec(ECGpart, fs, 512);
title('Spectrogram Original');
xlabel('Time');
ylabel('Frequency');
axis([0,5,0,200])
subplot(2,1,2)
plotspec(yy, fs, 512);
title('Spectrogram Filtered');
xlabel('Time');
ylabel('Frequency');
axis([0,5,0,200])
