%% Lab_05 Homework

% plot(tt, xx)
% subplot(1,3,3);
myvoice = wavread('myvoice.wav');
myvoice = myvoice(1:end/2 + 4000);
plot(myvoice)
figure
k2 = plotspecDB(myvoice, 15500, 31000, 80);
title('My Voice dB Spectrogram');
ylim([0,300]);
xlabel('Time (sec)');
ylabel('Frequency (Hz)');