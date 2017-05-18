%================================ segmentM ===============================
%
%  script segmentM
%
%
%  This is a Matlab script that will run Matlab's k-means algorithm
%  for segmenting an image.   You should select two images from the
%  homework Matlab file and perform segmentation on them. The output will 
%  be used by another script of mine for visualization.  You should also
%  pick a third color image of your own.
%
%
%  This code uses some Matlab tricks to be somewhat genecleric.  First,
%  all arguments are encapsulated into a cell array.  This works as
%  follows.  The cell array belows consists of two arguments:
%
%  >> sampleCellArray = {40, 34};
%
%  that when expanded as an argument to a function, provides two
%  inputs to the function,
%
%  >> plus( sampleCellArray{:} )
%
%  The output should be the addition of the two arguments:
%
%  ans =
%
%     74
%
%  Anyhow, this function expectes the same, but the arguments are
%  consistent with what the imkmeans function expects.
%
%================================ segmentK ===============================

load('segment.mat');
picks = [1 2];

for i = 1:length(picks)
  switch (picks(i))
    case 1,
      images{i} = westin;
      iparms{i} = { 6, 5 };
    case 2,
      images{i} = fish04;
      iparms{i} = { 6, 5 };
  end
end

% Add more cases if you want to do all of the images, and change 
% "picks" accordingly.

% Add your own personal case by including the code below:
% While the segmentation images above are grayscale, yours should
% be a color image file to change things up.
images{3} = imread('macaw.jpg');
iparms{3} = { 10, 6 };

for i = 1:length(images)
  [segimg{i}, K, nmeans{i}] = imkmeans(double(images{i}), iparms{i}{:});
  
  figure(3 * i);
  imagesc(images{i});
  axis image;
  colormap('gray');
  
  figure(3 * i + 1);
  imagesc(uint8(segimg{i}));
  axis image;
  colormap('default');
  
  figure(3 * i + 2);
  imagesc(uint8(K));
  axis image;
  colormap('gray');
  
  nmeans{i};
end

%
%================================ segmentK ===============================
