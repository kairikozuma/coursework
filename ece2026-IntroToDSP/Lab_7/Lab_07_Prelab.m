%% 2.4
xx = [ones(1,10), zeros(1,5)];
bb = (1/3)*ones(1,3);   
yy = firfilt(bb, xx);

nn = 1:length(xx);
subplot(2,1,1);
stem(nn - 1, xx(nn));
subplot(2,1,2);
stem(nn-1,yy(nn),'filled');
xlabel('Time Index (n)');
