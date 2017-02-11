function DCT = ConvertImgToDCT(filename,type)
% Example of function call 
%   DCT = ConvertImgToDCT('image','.tiff')
%
% This will result in 
%    1. variable DCT containing a DCT of the image in
%    2. A file 'image_DCT.csv' containing the DCT of image.tiff will be
%    written to disk
%    3. The image that was read being rendered in a figure
A = imread(strcat(filename,type)); 
DCT = dct2(A);
dlmwrite(strcat(filename,'_DCT.csv'),DCT,'delimiter',',','precision','%.6f');
figure;
imshow(A);
end
