function ConvertDCTFileToImg(filename)
% Example of function call 
%   DCT = ConvertDCTFileToImg('image_DCT.csv')
%
% This will result in a figure being generated in which the image obtained
% from taking an inverse DCT of the contents of image_DCT.csv 
Data = csvread(filename);
Image = idct2(Data);
figure;
imshow(Image,[0 255]);
end