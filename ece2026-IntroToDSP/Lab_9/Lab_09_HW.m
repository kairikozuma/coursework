%% Lab_09_HW
%% 4.1a
wp = .6*pi;
ws = .64*pi;
dp = .05;
ds = .01;

frp = wp/(pi);
frs = ws/(pi);

[N, Fo, Ao, W] = firpmord([frp, frs], [1, 0], [.05, .01], 2);
M = N + 1;
b = firpm(M, Fo, Ao, W);
fprintf('\nOrder used was M = %d, predicted M = %d', M, N);

%% 4.1b
n = 1:length(b);
stem(n-1,b);
title('Stem Plot of Impulse Response');
xlabel('Index (n)');
ylabel('Magnitude');

%% 4.1c
figure
ww = -pi:1/1000:pi;
HH = freqz(b, 1, ww);
plot(ww, abs(HH));
title('Frequency Response Magnitude');
xlabel('Normalized Frequency');
ylabel('Magnitude');

open HW_Mag.fig
open HW_Mag1.fig
open HW_Mag2.fig


%% 4.1e
figure
plot(ww, angle(HH));
title('Frequency Response Phase');
xlabel('Normalized Frequency');
ylabel('Phase');

open HW_phase.fig

slope = (-3.128 - 3.315)/(.08341 + .08359);
fprintf('\nSlope of FIR filter phase is %f', slope);