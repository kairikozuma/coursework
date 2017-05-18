

% Sample script for running diffusion once ...

smoothI = isodiffuse(IMAGENAME, DTVALUE, NUMITER);

fh = figure(1);
  clf;
  imagesc(IMAGENAME);
  colormap('gray');
  axis image;
  axis off;

fh = figure(2);
  clf;
  imagesc(smoothI);
  colormap('gray');
  axis image;
  axis off;
