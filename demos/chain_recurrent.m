% GAIO demo: chain recurrent set in the Henon map

% the Henon map
a = 1.4; b = 0.3;
f = @(x) [1-a*x(:,1).^2+x(:,2) b*x(:,1)];   

% choose sample points in reference square
n = 10; X1 = linspace(-1,1,n); 
[XX,YY] = meshgrid(X1,X1);
X = [XX(:) YY(:)];

% initialize the GAIO tree which stores the box covering
center = [0 0]; radius = [1.5 1.5]; 
tree = Tree(center, radius);

% compute the chain recurrent set
depth = 16;
crs(tree, f, X, depth);

% plot of the box covering
boxplot2(tree,'edgecolor','r'); 
