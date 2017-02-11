%% Lab_09
%% 3.1
% Load saved 30 point filter
load lp30.mat
w = -pi:1/8000:pi;

Hrec = freqz(lp30rec, 1, w);
subplot(2,1,1);
plot(w, abs(Hrec));
xlabel('Normalized Frequency');
ylabel('Magnitude');
title('30 Order Low Pass Filter');
Hham = freqz(lp30ham, 1, w);
subplot(2,1,2);
plot(w, abs(Hham));
xlabel('Normalized Frequency');
ylabel('Magnitude');
title('30 Order Low Pass Filter Hamming');


%% 3.2
Hham2 = freqz(lp60ham, 1, w);
figure;
plot(w, abs(Hham2));
xlabel('Normalized Frequency');
ylabel('Magnitude');
title('60 Order Low Pass Filter Hamming');

%% 3.3
Hham3 = freqz(lp153, 1, w);
figure;
plot(w, abs(Hham3));
xlabel('Normalized Frequency');
ylabel('Magnitude');
title('153 Order Low Pass Filter Hamming');
