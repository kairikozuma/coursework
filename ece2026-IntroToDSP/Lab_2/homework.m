x = 0;
t = -.3/40e6:1/(32*40e8):4/40e6;
f1 = cos(80e6*pi*(t-((100^2)^(1/2))/3e8));
f2 = cos(80e6*pi*(t-(40+(11600)^(1/2))/3e8));
f = f1 + f2;
plot(t, f)
grid on, zoom on
title('Sum of Sinusoids when x = 0m');
ylabel('Magnitude');
xlabel('Time (sec)');

simmat = [1,-j,exp(-j*(pi/5));1,exp(-j*(pi/9)),exp(j/6);1,1,1];
sol = [exp(-j*(pi/6)); -2*exp(-j*(pi/7));1.5];
ans = simmat\sol;
disp(ans)