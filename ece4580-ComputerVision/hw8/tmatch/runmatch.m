%================================ runmatch ===============================
%
%  Loads the template image and the image data from file, then invokes the
%  template matching algorithm to find the instance of that template in
%  the image.
%
%================================ runmatch ===============================

load('template.mat');				% Should be in your path.

[xpts, ypts] = tmatch(I, tI);

fh = figure(1);
  clf;
  imagesc(I);
  colormap('gray');
  hold on;
  plot(xpts, ypts,'c*','MarkerSize',10);
  hold off;

  axis off;

  
  
