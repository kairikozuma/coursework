%% 3.1
%% 3.2 a-d
fc = 770;
fdelta = 6;
fpass1 = fc - fdelta;
fpass2 = fc + fdelta;
fs = 1/0.3e-3;
wpass21 = 2*pi*fpass1/fs;
wpass22 = 2*pi*fpass2/fs;
delp = .02;
dels = .01;
fstop1 = 697;
fstop2 = 852;
wstop21 = 2*pi*fstop1/fs;
wstop22 = 2*pi*fstop2/fs;
w2delta1 = wpass21 - wstop21;
w2delta2 = wpass22 - wstop22;
[N2, Fo, Ao, W] = firpmord([fstop1, fpass1, fpass2, fstop2], [0, 1, 0], [dels, delp, dels], fs);
M2 = N2 - 1;
b = firpm(M2, Fo, Ao, W);

figure
ww = -pi:1/10000:pi;
HH = freqz(b, 1, ww);
plot(ww, abs(HH));
title('Frequency Response Magnitude');
xlabel('Normalized Frequency');
ylabel('Magnitude');

figure
subplot(2,1,1);
xx = DTMFdial('159D*86A', fs);
plotspec(xx,fs);
subplot(2,1,2);
yy2 = conv(xx, b);
plotspec(yy2,fs);

%% 3.2 e
fc = 1209;
fdelta = 6;
fpass1 = fc - fdelta;
fpass2 = fc + fdelta;
fs = 1/0.3e-3;
wpass51 = 2*pi*fpass1/fs;
wpass52 = 2*pi*fpass2/fs;
delp = .02;
dels = .01;
fstop1 = fc - 127;
fstop2 = fc + 127;
wstop51 = 2*pi*fstop1/fs;
wstop52 = 2*pi*fstop2/fs;
[N5, Fo, Ao, W] = firpmord([fstop1, fpass1, fpass2, fstop2], [0, 1, 0], [dels, delp, dels], fs);
M5 = N5 -2;
b = firpm(M5, Fo, Ao, W);

figure
ww = -pi:1/10000:pi;
HH5 = freqz(b, 1, ww);
plot(ww, abs(HH5));
title('Frequency Response Magnitude');
xlabel('Normalized Frequency');
ylabel('Magnitude');

figure
subplot(2,1,1);
xx = DTMFdial('159D*86A', fs);
plotspec(xx, fs);
subplot(2,1,2);
yy5 = conv(xx, b);
plotspec(yy5, fs);

%% 3.2 f
fc = 1633;
fdelta = 6;
fpass1 = fc - fdelta;
fs = 1/0.3e-3;
wpass81 = 2*pi*fpass1/fs;
delp = .02;
dels = .01;
fstop1 = fc - 156;
wstop81 = 2*pi*fstop1/fs;
[N8, Fo, Ao, W] = firpmord([fstop1, fpass1], [0, 1], [dels, delp], fs);
M8 = N8 + 6;
b = firpm(M8, Fo, Ao, W);

figure
ww = -pi:1/10000:pi;
HH = freqz(b, 1, ww);
plot(ww, abs(HH));
title('Frequency Response Magnitude');
xlabel('Normalized Frequency');
ylabel('Magnitude');

figure
subplot(2,1,1);
xx = DTMFdial('159D*86A', fs);
plotspec(xx,fs);
subplot(2,1,2);
yy8 = conv(xx, b);
plotspec(yy8,fs);

%% 3.3
yy2fin = firfilt([zeros(1,M2/2)],yy2);
yy5fin = firfilt([zeros(1,M5/2)],yy5);
yy8fin = firfilt([zeros(1,M8/2)],yy8);

%% 3.4
Lp = 99;
w = -pi:pi/100:pi;
hh = hamming(Lp);
figure
H = freqz(hh, 1, w);
n = (1:length(H)) - round(length(H)/2);
plot(n, abs(H));
R = round(Lp/4);
%pDownSampled = avgPower(1:R:end);


correction = [-7, -1, 2, 0, -4, 0, -3, 6];
