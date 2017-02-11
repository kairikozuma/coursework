%% Lab_06

%% 3.2
A = 2;
fc = 800;
al = 1000;
be = 1.5;
ga = 0;
tt = 0:1/4000:2;
x = A*cos(2*pi*fc*tt + al*cos(2*pi*be*tt + ga));
plotspec(x+j*1e-12, 4000, 128);

%% 3.3
xpix = ones(256, 1)*cos(2*pi*(0:255)/32);
imshow(xpix);
% white is 1
% black is any value less than 0
% xpixalt = xpix(:,1:9);
% imshow(xpixalt);

%% 3.4 a
wd = 2*pi*(1/32);
wd2 = 2*pi*(2/32);
wd14 = 2*pi*(14/32);
xpix = ones(256,1)*cos(wd*(0:255));
xpix2 = ones(256, 1)*cos(wd2*(0:255));
xpix14 = ones(256, 1)*cos(wd14*(0:255));
subplot(1,3,1)
imshow(xpix)
title('f = 32 Hz');
subplot(1,3,2)
imshow(xpix2)
title('f = 16 Hz');
subplot(1,3,3)
imshow(xpix14)
title('f = 16/7 Hz');

%% 3.5 b
xpix2down = xpix2(1:2:end,1:2:end);
xpix14down = xpix14(1:2:end,1:2:end);
subplot(2,2,1)
imshow(xpix2)
title('Xpix2');
subplot(2,2,2)
imshow(xpix14)
title('Xpix14');
subplot(2,2,3)
imshow(xpix2down)
title('Xpix2 Down Sampled');
subplot(2,2,4)
imshow(xpix14down)
title('Xpix14 Down Sampled');