%%%% 2.4 %%%%
fsamp = 8000;
dt = 1/fsamp;
dur = 2;
tt = 0:dt:dur;
f1 = 400;
psi = 2*pi*(100 + f1*tt + 50*tt.^2);
xx = real(7.7*exp(j*psi));
soundsc(xx,5*fsamp);

