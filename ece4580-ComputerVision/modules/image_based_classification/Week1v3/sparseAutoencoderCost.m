function [cost,grad] = sparseAutoencoderCost(theta, visibleSize, hiddenSize, ...
                                             lambda, sparsityParam, beta, data)

% visibleSize: the number of input units (probably 64) 
% hiddenSize: the number of hidden units (probably 25) 
% lambda: weight decay parameter
% sparsityParam: The desired average activation for the hidden units (denoted in the lecture
%                           notes by the greek alphabet rho, which looks like a lower-case "p").
% beta: weight of sparsity penalty term
% data: Our 64x10000 matrix containing the training data.  So, data(:,i) is the i-th training example. 
  
% The input theta is a vector (because minFunc expects the parameters to be a vector). 
% We first convert theta to the (W1, W2, b1, b2) matrix/vector format, so that this 
% follows the notation convention of the lecture notes. 

W1 = reshape(theta(1:hiddenSize*visibleSize), hiddenSize, visibleSize);
W2 = reshape(theta(hiddenSize*visibleSize+1:2*hiddenSize*visibleSize), visibleSize, hiddenSize);
b1 = theta(2*hiddenSize*visibleSize+1:2*hiddenSize*visibleSize+hiddenSize);
b2 = theta(2*hiddenSize*visibleSize+hiddenSize+1:end);

% Cost and gradient variables (your code needs to compute these values). 
% Here, we initialize them to zeros. 
cost = 0;
W1grad = zeros(size(W1)); 
W2grad = zeros(size(W2));
b1grad = zeros(size(b1)); 
b2grad = zeros(size(b2));

%% ---------- YOUR CODE HERE --------------------------------------
%  Instructions: Compute the cost/optimization objective J_sparse(W,b) for the Sparse Autoencoder,
%                and the corresponding gradients W1grad, W2grad, b1grad, b2grad.
%
% W1grad, W2grad, b1grad and b2grad should be computed using backpropagation.
% Note that W1grad has the same dimensions as W1, b1grad has the same dimensions
% as b1, etc.  Your code should set W1grad to be the partial derivative of J_sparse(W,b) with
% respect to W1.  I.e., W1grad(i,j) should be the partial derivative of J_sparse(W,b) 
% with respect to the input parameter W1(i,j).  Thus, W1grad should be equal to the term 
% [(1/m) \Delta W^{(1)} + \lambda W^{(1)}] in the last block of pseudo-code in Section 2.2 
% of the lecture notes (and similarly for W2grad, b1grad, b2grad).
% 
% Stated differently, if we were using batch gradient descent to optimize the parameters,
% the gradient descent update to W1 would be W1 := W1 - alpha * W1grad, and similarly for W2, b1, b2. 
% 
% 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%% Not Vectorized %%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% [~,m] = size(data); 
% for i = 1:m
%     % 1. Perform forward pass to get output layer activation energies
%     x = data(:,i);
%     z2 = W1*x+b1;
%     a2 = sigmoid(z2); % f(z) = 1/(1+exp(-z)), which is the sigmoid function
%     z3 = W2*a2+b2;
%     a3 = sigmoid(z3); % This is the output activation function.
%     
%     % 0.5 * Sum up all the difference squared
%     diff = a3-x;
%     cost_temp = 0.5 * sum(diff.^2)/m;
%     cost = cost+cost_temp;
%     
%     % 2. Compute the output layer error, relative to original data
%     f_prime_z3 = sigmoid(z3).*(1-sigmoid(z3));
%     e3 = -(x-a3).*f_prime_z3; % true value should be x, or equivalently y in the note
%     % 3. Back-propagation to calculate inner layer errors
%     f_prime_z2 = sigmoid(z2).*(1-sigmoid(z2));
%     e2 = ((W2)'*e3) .* f_prime_z2;
%     % 4. Compute desired partial derivatives for all layers
%     partial_W1_J = e2*x';
%     partial_W2_J = e3*a2';
%     partial_b1_J = e2;
%     partial_b2_J = e3;
%     
%     W1grad = W1grad+partial_W1_J;
%     W2grad = W2grad+partial_W2_J;
%     b1grad = b1grad+partial_b1_J;
%     b2grad = b2grad+partial_b2_J;
% end
% 
% W1grad = W1grad/m;
% W2grad = W2grad/m;
% b1grad = b1grad/m;
% b2grad = b2grad/m;
% 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%% Vectorized %%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[~,m] = size(data); 
% 1. Perform forward pass to get output layer activation energies
x = data;
z2 = W1*x+repmat(b1,1,m);
a2 = sigmoid(z2); % f(z) = 1/(1+exp(-z)), which is the sigmoid function
z3 = W2*a2+repmat(b2,1,m);
a3 = sigmoid(z3); % This is the output activation function.

% Determine cost function J
% A) Average sum of squares error term
% 0.5 * Sum up all the difference squared
diff = a3-x;
sqrdiff = (0.5/m) * sum(sum(diff.^2));

% B) weight decay term (regularization)
% (lambda / 2) * triple summation of weights^2
%decay = (lambda / 2.0) * (sum(sum(W1.^2) + sum(sum(W2.^2))));
decay = (lambda / 2.0) * (sum(sum(W1.^2)) + sum(sum(W2.^2)));

% C) Sparsity term (KL-divergence)
p = sparsityParam;
p_j = (1/m) * sum(a2,2);
KL = sum(p*log(p./p_j)+(1-p)*log((1-p)./(1-p_j)));
penalty = beta * KL;

%%%%%%%%%%%%%%%%%%%%%
% Final cost function
%%%%%%%%%%%%%%%%%%%%%
cost = sqrdiff + decay + penalty;

% 2. Compute the output layer error, relative to original data
f_prime_z3 = a3.*(1-a3);
e3 = -(x-a3).*f_prime_z3; % true value should be x, or equivalently y in the note

% 3. Back-propagation to calculate inner layer errors
f_prime_z2 = a2.*(1-a2);
% e2 = ((W2)'*e3) .* f_prime_z2;
p_temp = beta * ((-p./p_j) + (1-p)./(1-p_j));   % 25 by 1
p_temp = repmat(p_temp,1,m);    % 25 by m
e2 = (((W2)'*e3) + p_temp ) .* f_prime_z2;

% 4. Compute desired partial derivatives for all layers
W1grad = (e2*x')./m + lambda .* W1;
W2grad = (e3*a2')./m + lambda .* W2;
b1grad = sum(e2,2)./m;   % due to dot product, we have to manually sum each row
b2grad = sum(e3,2)./m;   % due to dot product, we have to manually sum each row

%-------------------------------------------------------------------
% After computing the cost and gradient, we will convert the gradients back
% to a vector format (suitable for minFunc).  Specifically, we will unroll
% your gradient matrices into a vector.

grad = [W1grad(:) ; W2grad(:) ; b1grad(:) ; b2grad(:)];

end

%-------------------------------------------------------------------
% Here's an implementation of the sigmoid function, which you may find useful
% in your computation of the costs and the gradients.  This inputs a (row or
% column) vector (say (z1, z2, z3)) and returns (f(z1), f(z2), f(z3)). 

function sigm = sigmoid(x)
  
    sigm = 1 ./ (1 + exp(-x));
end

