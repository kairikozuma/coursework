function bnum = bar2num(filename)
% Converts image of barcode into 12 digit barcode number
% Input: (string) name of barcode image
% Output:(double) barcode output

% Read in the image
bar = imread(filename);

% Extract a single row from image and find first difference filter
[row, col] = size(bar);
mid = floor(row/2);
xbar = double(bar(mid,:));
bk = [1 -1];
ybar = firfilt(bk, xbar);

% Filter out any values with absolute value less than threshold
% Filter out all zero values (skip 1st edge because it is white)
det = abs(ybar) > 150;
loc = find(det(2:end) ~= false);

% subplot(2,1,2);
locdelt = firfilt(bk, loc);

% Calculate the average theta value
theta = zeros(1,length(locdelt) - 59);
for i = 1:length(theta)
    theta(i) = sum(locdelt(i:58 + i))/95;
end
theta1 = sum(theta)/length(theta);

% Calculate the relative signal (all 1,2,3,4)
locdeltrel = round(locdelt(1:end-1)/theta1);

% Find location of vector [1 1 1] within barcode
% Location is starting point for barcode
[~, sloc] = ismember([1,1,1], locdeltrel);
barfilt = locdeltrel(sloc(1):sloc(1) + 58);
stem(barfilt);
barnum = decodeUPC(barfilt);
barnumfin = sum(10.^(length(barnum)-1:-1:0) .* barnum);

% Set output
bnum = barnumfin;

% Print output
fprintf('\nBarcode is: %d\n', barnumfin);
end
