%% 2.1
ham = hamming(23);
range = 1:23;
stem(range - 1, ham(range));

[val, loc] = max(ham);
loc = loc - 1;
fprintf('\nThe maximum value is %d at n = %d\n', val, loc);
val0 = ham(1);
val11= ham(12);
val22= ham(22);

%% 2.2
m = 22;
n = 0:m;
h1 = sin(0.7*pi*(n - 11))./(pi*(n - 11));
n = 1:length(h1);
wr = ones(1,m+1);
h2 = wr.*h1;
subplot(2,1,1);
stem(n, h2);
w = -pi:1/100:pi;
H = freqz(h2, 1, w);
subplot(2,1,2);
plot(w, H);

wm = hamming(23);
h3 = wm'.*h1;
n = 1:length(h3);
figure;
stem(n-12, h3);