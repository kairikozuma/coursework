close all;
subplot(1,2,1)
Data = csvread('ChemFactory_DCT.csv');
Image = idct2(Data);
imshow(Image,[0 255]);

subplot(1,2,2)
Data = csvread('ChemFactory_DCT_new_168.csv');
Image = idct2(Data);
imshow(Image,[0 255]);
