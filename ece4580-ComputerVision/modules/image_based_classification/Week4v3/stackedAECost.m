function [ cost, grad ] = stackedAECost(theta, inputSize, hiddenSize, ...
                                              numClasses, netconfig, ...
                                              lambda, data, labels)
                                         
% stackedAECost: Takes a trained softmaxTheta and a training data set with labels,
% and returns cost and gradient using a stacked autoencoder model. Used for
% finetuning.
                                         
% theta: trained weights from the autoencoder
% visibleSize: the number of input units
% hiddenSize:  the number of hidden units *at the 2nd layer*
% numClasses:  the number of categories
% netconfig:   the network configuration of the stack
% lambda:      the weight regularization penalty
% data: Our matrix containing the training data as columns.  So, data(:,i) is the i-th training example. 
% labels: A vector containing labels, where labels(i) is the label for the
% i-th training example


%% Unroll softmaxTheta parameter

% We first extract the part which compute the softmax gradient
softmaxTheta = reshape(theta(1:hiddenSize*numClasses), numClasses, hiddenSize);

% Extract out the "stack"
stack = params2stack(theta(hiddenSize*numClasses+1:end), netconfig);

% You will need to compute the following gradients
softmaxThetaGrad = zeros(size(softmaxTheta));
stackgrad = cell(size(stack));
for d = 1:numel(stack)
    stackgrad{d}.w = zeros(size(stack{d}.w));
    stackgrad{d}.b = zeros(size(stack{d}.b));
end

cost = 0; % You need to compute this

% You might find these variables useful
m = size(data, 2);
groundTruth = full(sparse(labels, 1:m, 1));


%% --------------------------- YOUR CODE HERE -----------------------------
%  Instructions: Compute the cost function and gradient vector for 
%                the stacked autoencoder.
%
%                You are given a stack variable which is a cell-array of
%                the weights and biases for every layer. In particular, you
%                can refer to the weights of Layer d, using stack{d}.w and
%                the biases using stack{d}.b . To get the total number of
%                layers, you can use numel(stack).
%
%                The last layer of the network is connected to the softmax
%                classification layer, softmaxTheta.
%
%                You should compute the gradients for the softmaxTheta,
%                storing that in softmaxThetaGrad. Similarly, you should
%                compute the gradients for each layer in the stack, storing
%                the gradients in stackgrad{d}.w and stackgrad{d}.b
%                Note that the size of the matrices in stackgrad should
%                match exactly that of the size of the matrices in stack.
%

% AE Feed Forward
W1 = stack{1}.w;
W2 = stack{2}.w;
b1 = stack{1}.b;
b2 = stack{2}.b;

z2 = W1*data+repmat(b1,1,m);
a2 = sigmoid(z2);
z3 = W2*a2+repmat(b2,1,m);
a3 = sigmoid(z3); 

M = softmaxTheta*a3;
M = bsxfun(@minus, M, max(M, [], 1));
M = exp(M);
M = bsxfun(@rdivide, M, sum(M));
h = M;

numCases = size(a3, 2);
% output diff^2 + the weight decay term
SMcost = (-1/numCases) * sum(sum(groundTruth.*log(h)))+sum(sum(softmaxTheta.^2)).*lambda./2;
softmaxThetaGrad = (-1/numCases) *(groundTruth - h)* a3'+lambda*softmaxTheta ;

% Back Propagation from Softmax
f_prime_z3 = a3.*(1-a3);
e3 = -(softmaxTheta'*(groundTruth-h)).*f_prime_z3;
f_prime_z2 = a2.*(1-a2);
e2 = ((W2)'*e3) .* f_prime_z2;

% 4. Compute desired partial derivatives for all layers
stackgrad{1}.w = (e2*data')./m;  % + lambda .* W1;
stackgrad{2}.w = (e3*a2')./m;    % + lambda .* W2;
stackgrad{1}.b = sum(e2,2)./m;   % due to dot product, we have to manually sum each row
stackgrad{2}.b = sum(e3,2)./m;   % due to dot product, we have to manually sum each row

% Compute total cost
cost = SMcost;
%% Roll gradient vector
grad = [softmaxThetaGrad(:) ; stack2params(stackgrad)];

end


% You might find this useful
function sigm = sigmoid(x)
    sigm = 1 ./ (1 + exp(-x));
end
