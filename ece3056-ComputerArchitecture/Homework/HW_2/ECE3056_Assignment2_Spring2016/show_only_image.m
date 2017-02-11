Data = csvread('ChemFactory_DCT_new_168.csv');
Image = idct2(Data);
imshow(Image,[0 255], 'Border', 'tight');