%================================ segBayes ===============================
%
%  function [J, K, means] = segBayes(I, iter, means, stdvs)
%
%  Perform Bayesian segmentation on the image I.
%
%  Input:
%    I		- image I.
%    iter	- the number of iterations. if negative, then go to convergence.
%    means	- initial guess at means (column-wise).
%    stdvs	- optional input of standard deviations.
%             can be scalar for all means to use, or can be a unique
%             value for each mean value.
%
%  Output:
%    J		- the segmentation map.
%    K		- the simplified image using the means and the segmentation.
%    means	- the Gaussian means for each class.
%    stdvs	- the Gaussian standard deviations for each class.
%    iter	- number of iterations (if < 0, then to convergence).
%
%================================ segBayes ===============================
function [J, K, means] = segBayes(I, iter, means, stdvs)

%--[0] Parse the input arguments, set to defaults if needed.
if (nargout == 0)       % If nothing expected, then don't bother
  return;               %   doing the computations.
end

if (nargin < 4)         % If first three not given, can't do much.
  disp('ERROR: Need at least the first three arguments');
  error('BadArgs');
end

if (isempty(iter))				% If iterations not given,
  iter = -1;					%  then go to convergence.
end

if ((nargin < 4) || isempty(stdvs))		% If no argument or if empty.
  stdvs = 1;
end

if (isscalar(stdvs))					% If scalar, vectorize.
  stdvs = repmat(stdvs, [1, size(means, 2)]);
end

%--[1] Prep workspace and variables.
[M, N] = size(I);               % Get problem dimensions.
P = length(means);

                        		% Homogeneous priors.
priors = reshape(ones([M*N,P])*diag(ones(P,1)/P), [M, N, P]);
image  = I(:,:,ones(P,1));		% Duplicate image I for quick evaluation.

								% Init. space for (posterior) probabilities.
post   = reshape(ones([M*N,P])*diag(ones(P,1)/P), [M, N, P]);

%--[2] Setup, then perform the segmentation iterations.
hasConverged = false;
oJ = zeros([M, N]);
mold = means;

while ( ((iter < 0) && (~hasConverged)) || ( iter > 0) )

  % YOUR CODE HERE.
  % Steps:
  %  (1) Calculate likelihoods using normal distribution.
  post = compLikelihoods();
  
  %  (2) Multiply by priors to generate posterior "probs".
  post = post .* priors;

  %  (3) Normalize posteriors.
  normalizeProb();

  %  (4) Smooth posteriors (use heat/diffusion or Gaussian smoothing).
  post = smoothProb(post);
  
  %  (5) Normalize smoothed posteriors.
  normalizeProb();

  %  (6) Find maximum posterior for classification/segmentation.
  [val, J] = max(post,[],3);
  
  %  (7) Update means.
  compMeans();
  
  %  (8) Set priors to be posteriors.
  priors = post;

  %  (8) Termination conditions and loop update.
  if all(mold==means)				% All new means equal to old means.
    hasConverged = true;
  elseif all(oJ(:) == J(:))			% Old segmentation equals new segmentation.
    hasConverged = true;
  end

  if (iter > 0)
    iter = iter - 1;
  end

end

ncov = stdvs .^ 2;
if (nargout >= 2)
  K = J;
  means = means .* ncov;
  for i=1:size(means,2)
      K(J == i) = means(i);
  end
end

%
%============================ Helper Functions ===========================
%
%  These functions live within the scope of the segBayes function.
%  What that means is that they can be invoked from the loop above
%  and they will have access to the variables above (think of them
%  as global variables in some sense).  Using functions within a 
%  function is a clean way to do complex thing but have the main
%  code of the function look nice and clean.
%

  %-------------------------- compLikelihoods --------------------------
  %
  %  Computes the likelihoods for Bayesian inference.
  %
  function likelies = compLikelihoods

  likelies = zeros([M, N, P]);		% Initialize.
  for li = 1:P
    likelies(:,:,li) = normpdf(I,repmat(means(li),M,N),repmat(stdvs(li),M,N)); %TODO
  end

  end
  %----------------------------- smoothProb ----------------------------
  %
  %  Smooth the probabilities as part of the optimization step in Bayesian
  %  segmentation.  You can invoke whatever function you would like here.
  %  If you have seprate code to do that, just insert the function call here
  %  with all of the parameters set properly.
  %
  %  This function can either smooth each individual probability field (for
  %  each of the classes), or smooth all of them at once.  It is up to you
  %  to decide that part and code things appropriately above.  
  %
  function iprob = smoothProb(iprob)

  % Invoke your favorite smoother here to smooth the probabilities.
  sigma = 0.5;
  iprob = imgaussfilt(iprob, sigma);

  end

  %--------------------------- normalizeProbs --------------------------
  %
  %  Normalize the posterior probabilities.
  %
  function normalizeProb

  tmp  = sum(post,3) + eps;     % Compute sum.  Add epsilon to avoid divide
                                %  by zero.
  tmp  = tmp(:,:,ones(P,1));
  post = post./tmp;             % Divide by sum (plus epsilon).

  end

  %----------------------------- compMeans -----------------------------
  %
  %  Given the segmentation, compute the class means.  The mean should
  %  be updated only if there is more than 1 pixel in the class.
  %
  function compMeans
  for mi=1:length(means)				% For each class label ...
    pts_in_layer = sum(sum(J == mi));
    if (pts_in_layer > 0)
        means(mi) = sum(I(J == mi)) ./ pts_in_layer;
    else
        means(mi) = 0;
    end
  end
  end

end

%
%================================ segBayes ===============================
