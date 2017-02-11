%%%% Lab 3 HW %%%%

%%%% Part 4.1a %%%%
fs = 4000;
dur = 4.04;
timesec = .07;
testingBeatIn = struct('Amp', 66, 'fc', 900, 'phic', 0, 'fDelt', 40, 'phiDelt', pi/4, 't1', 0, 't2', dur, 'fs', fs);
testingBeat = sum2BeatStruct(testingBeatIn);
plot(testingBeat.times(1:timesec*fs), testingBeat.values(1:timesec*fs));
title('testingBeat AM Signal');
xlabel('Time (seconds)');
ylabel('Magnitude');

%%%% Part 4.1c %%%%
figure;
plotspec(testingBeat.values+j*1e-12,fs,256);
grid on, shg
title('testingBeat Spectrogram');
xlabel('Time (sec)');
ylabel('Frequency (Hz)');

%%%% 4.2 %%%%
period = .0003;
xx = DTMFdial('159D*86A',1/period);
figure;
plotspec(xx + j*1e-12,1/period, 256);
title('Plotspec of DTMFdial "159D*86A"');
xlabel('Time (seconds)');
ylabel('Frequency (Hz)');
