%% GAIO demo: relative global attractor of the Henon map

% the Henon map 
a = 1.4; b = 0.3;                                       % parameters
f = @(x) [1-a*x(:,1).^2+x(:,2)  b*x(:,1)];              % the map

% choose sample points in [-1,1]^2
n = 40; X1 = linspace(-1,1,n)'; E = ones(size(X1));     % 1d points
X = [ X1 -E; X1 E; -E X1; E X1];                        % sample points in [-1,1]^2

% initialize the GAIO tree which stores the box covering
center = [0 0]; radius = [3 1];                         
tree = Tree(center, radius);

% computation of the relative global attractor
depth = 16;                                             % final depth of the tree
rga(tree, f, X, depth);

% plot of the box covering
boxplot2(tree);
