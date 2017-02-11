%% #1
Ts = 0.3/1000;
fs = 1/Ts;
dp = 0.02;
ds = 1/100;
[m,fo,mo,w] = firpmord( [624, 697-6, 697+6 , 770], [0, 1 0], [ds dp ds], fs);
b1 = firpm(m-9,fo,mo,w);

ww = -pi:(pi/1000):pi;
h = freqz(b1,1,ww);

ww1 = ww*(fs)/(2*pi);
plot(ww1,abs(h));
grid on

M=[];M=[M,m-9]
b={b1};
%%  #2
[m,fo,mo,w] = firpmord( [697, 764, 776 , 852], [0, 1 0], [ds dp ds], fs);
b2 = firpm(m-11,fo,mo,w);
h = freqz(b2,1,ww);
plot(ww1,abs(h));
grid on

M = [M, m-11];
b{2}=b2;
%% #3
[m,fo,mo,w] = firpmord( [770, 852-6, 852+6 , 941], [0, 1 0], [ds dp ds], fs);
b3 = firpm(m-8,fo,mo,w);
h = freqz(b3,1,ww);
plot(ww1,abs(h));
grid on
b{3}=b3;
M = [M, m-8];
%% #4
[m,fo,mo,w] = firpmord( [852, 941-6, 941+6 , 1030], [0, 1 0], [ds dp ds], fs);
b4 = firpm(m-6,fo,mo,w);
h = freqz(b4,1,ww);
plot(ww1,abs(h));
grid on
M=[M, m-6];
b{4}=b4;
%% Filter #5
[m,fo,mo,w] = firpmord( [941+121, 1209-6, 1209+6 , 1336], [0, 1 0], [ds dp ds], fs);
b5 = firpm(m-6,fo,mo,w);
h = freqz(b5,1,ww);
plot(ww1,abs(h));
grid on
M = [M, m-6];
b{5}=b5;
%% #6
[m,fo,mo,w] = firpmord( [1209, 1336-6, 1336+6 , 1477], [0, 1 0], [ds dp ds], fs);
b6 = firpm(m-6,fo,mo,w);
h = freqz(b6,1,ww);
plot(ww1,abs(h));
grid on
M = [M, m-6];
b{6}=b6;
%% #7
[m,fo,mo,w] = firpmord( [1336, 1477-6, 1477+6 , 1633], [0, 1 0], [ds dp ds], fs);
b7 = firpm(m-5,fo,mo,w);
h = freqz(b7,1,ww);
plot(ww1,abs(h));
grid on
M=[M, m-5];
b{7}=b7;
%% Filter #8 
[m,fo,mo,w] = firpmord( [1477, 1633-6], [0, 1], [ds dp], fs);
b8 = firpm(m+2,fo,mo,w);
h = freqz(b8,1,ww);
plot(ww1,abs(h));
grid on
M = [M, m+2];
b{8}=b8;
%% hw 4.2(a)
keyNames = '159D*86A0';
xx = DTMFdial (keyNames,fs);
figure
plotspec(xx,fs)
XX1 = firfilt(xx,b1);
subplot(4,2,1)
plotspec(XX1,fs)

keyNames = '159D*86A0';
XX2 = firfilt(xx,b2);
subplot(4,2,2)
plotspec(XX2,fs)

keyNames = '159D*86A0';
XX3 = firfilt(xx,b3);
subplot(4,2,3)
plotspec(XX3,fs)

keyNames = '159D*86A0';
XX4 = firfilt(xx,b4);
subplot(4,2,4)
plotspec(XX4,fs)

keyNames = '159D*86A0';
XX5 = firfilt(xx,b5);
subplot(4,2,5)
plotspec(XX5,fs)


keyNames = '159D*86A0';
XX6 = firfilt(xx,b6);
subplot(4,2,6)
plotspec(XX6,fs)


keyNames = '159D*86A0';
XX7 = firfilt(xx,b7);
subplot(4,2,7)
plotspec(XX7,fs)

keyNames = '159D*86A0';
XX8 = firfilt(xx,b8);
subplot(4,2,8)
plotspec(XX8,fs)


M = 0.5*M;
maxorder = max(M);
diff = maxorder - M;

for i = 1:length(M)
    b{i} = firfilt( [zeros(1,diff(i)), 1], b{i});
    
end
    
%% 4.2 (b)



% maxl = max(l);
% ldiff = maxl - l;
% 
% newp = [];
% for i =1:8
%     p{i} = [p{i},zeros(1,ldiff(i))];
%     newp(:,i) = [p{i}'];
% end

LP = round(0.048*fs/3);
hh = hamming(LP);
R = round(LP/4);

p1sq = XX1.^2;
p2sq = XX2.^2;
p3sq = XX3.^2;
p4sq = XX4.^2;
p5sq = XX5.^2;
p6sq = XX6.^2;
p7sq = XX7.^2;
p8sq = XX8.^2;

avg1 = firfilt(p1sq,hh);
avg2 = firfilt(p2sq,hh);
avg3 = firfilt(p3sq,hh);
avg4 = firfilt(p4sq,hh);
avg5 = firfilt(p5sq,hh);
avg6 = firfilt(p6sq,hh);
avg7 = firfilt(p7sq,hh);
avg8 = firfilt(p8sq,hh);

avg1 = avg1(1:R:end)';
avg2 = avg2(1:R:end)';
avg3 = avg3(1:R:end)';
avg4 = avg4(1:R:end)';
avg5 = avg5(1:R:end)';
avg6 = avg6(1:R:end)';
avg7 = avg7(1:R:end)';
avg8 = avg8(1:R:end)';

avg3 = [avg3, 0];
avg4 = [avg4, 0];
avg5 = [avg5, 0,0,0];
avg6 = [avg6, 0,0,0];
avg7 = [avg7, 0,0,0];
avg8 = [avg8, 0,0,0];

avgPower = [avg1; avg2; avg3; avg4; avg5; avg6; avg7; avg8]';
figure
strips(avgPower,fs/R);
phoneNumber = decodeDTMF(avgPower)
