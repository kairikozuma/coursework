%% Lab_07
% dconvdemo



% x[n] -3, 6, 2
% h[n] 1, -1

% x[n] 2 7 0
% h[n] 3 4 0

% double((sin(0.5*(0:50))+0.2) < 0 )

load echart.mat
bdiffh = [1 -1];
m = 65;
row = echart(m,:);
yy1 = conv(row, bdiffh);
nn = 1:length(row);
subplot(3,1,1);
stem(nn - 1, row(nn));
title(sprintf('Row %d',m));
xlabel('Column Index (n)');
subplot(3,1,2);
stem(nn - 1, yy1(nn), 'filled');
title(sprintf('Row %d First Difference Filter',m));
xlabel('Column Index (n)');
subplot(3,1,3);
imshow(row);
title(sprintf('Image of Row %d', m));
xlabel('Column Index (n)');

changeloc = find(yy1(2:end) ~= 0);
widthE = changeloc(2) - changeloc(1);
fprintf('Width of E is %d\n', widthE);
