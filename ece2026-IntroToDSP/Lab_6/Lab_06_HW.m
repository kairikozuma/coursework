%% Lab_06 Homework
%% 4.1
img = imread('lighthouse.png');
imgdown = img(1:2:end,1:2:end);
% subplot(1,2,1)
show_img(img)
trusize
title('Original Image');
% subplot(1,2,2)
show_img(imgdown);
trusize
title('Downsampled Image');

%% 4.2
img2 = imread('greek.jpg');
img2res = img2(1:768,1:1024,:);
img2gr = rgb2gray(img2res);
img2grdown = img2gr(1:3:end,1:3:end);
img2grdown2 = img2grdown(1:3:end,1:3:end);
% subplot(1,2,1)
show_img(img2gr)
trusize
title('Original Image');
% subplot(1,2,2)
show_img(img2grdown)
trusize
title('Downsampled Image');