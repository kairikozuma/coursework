%==========================
%  Name:		hw2_2.m
%
%  Author:		Kairi Kozuma
%
%==========================

% Histogram used to choose threshold
% hist(reshape(double(range), 1, 480*640), 2200);

% Set threshold values
lowerThresh = 959;
upperThresh = 978;


% Apply threshold to range image
binImage = (range > lowerThresh) & (range < upperThresh);

% bwselect to pick out person
binPerson = bwselect(binImage, 304, 212);

figure(1);
imshow(binPerson);
title('Filtered binary image of person');