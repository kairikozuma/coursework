%%%% 2.6 %%%%
amps = [100,200];
freqs = [800,2000];
phases = [.6*pi, -.1*pi];
fs = 8000;
tStart = [0.1,0.1];
durs = [0.4,0.4];
maxTime = max(tStart + durs) + 0.1;
durLengthEstimate = ceil(maxTime*fs);
tt = (0:durLengthEstimate)*(1/fs);
% Vector of Zeros to hold total signal
xx = 0*tt;
% Sum vectors
for kk = 1:length(amps)
    nStart = round(tStart(kk)*fs) + 1;
    xNew = shortSinus(amps(kk), freqs(kk), phases(kk), fs, durs(kk));
    Lnew = length(xNew);
    nStop = (nStart + Lnew - 1);
    xx(nStart:nStop) = xx(nStart:nStop) + xNew;
end
plotspec(xx, fs, 256);

% Concatenate Vectors
yy = [];
bl = zeros(1,fs*0.1);
for kk = 1:length(amps)
    yNew = shortSinus(amps(kk), freqs(kk), phases(kk), fs, durs(kk));
    yy = [yy bl yNew];
end
tt = (1/fs)*(0:length(yy)-1);
soundsc(yy, fs);
figure;
plot(tt,yy);

% Spectrogram
fs = 8000;
xx = cos(2000*pi*(0:1/fs:0.5));
plotspec(xx+j*1e-9, fs, 1024); colorbar

