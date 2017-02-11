%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% The purpose of this script is to demonstrate the use of the functions
% ConvertImgToDCT,
% ConvertDCTFileToImg,
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
close all;
DCT = ConvertImgToDCT('AerialView','.tiff');
ConvertDCTFileToImg('AerialView_DCT.csv');