%% Lab_08_Prelab
%% 2.1
bb = [0.5, 0.5];
ww = -pi:(pi/100):pi;
H = freqz(bb, 1, ww);
subplot(2,1,1);
plot(ww, abs(H)), grid on
subplot(2,1,2);
plot(ww, angle(H)), grid on
xlabel('Normalized Radian Frequency');

%% 2.3
ww = -pi:(pi/200):pi;
Cw = (2*cos(0.5*ww) + 2*cos(1.5*ww))/4;
psi = -1.5*ww;
subplot(2,1,1)
plot(ww, Cw);
hold on
plot(ww, abs(Cw), 'r--');
title('C(w)');
xlabel('w');

subplot(2,1,2)
title('psi(w)');
plot(ww, psi);

%% 2.3c
bb = 1/4*ones(1,4);
H = freqz(bb, 1, ww);
figure
subplot(2,1,1)
plot(ww, H);
hold on
plot(ww, abs(H), 'r--');
title('H');
xlabel('w');

subplot(2,1,2)
title('psi(w)');
plot(ww, angle(H));

%% 2.5


%% 2.4
