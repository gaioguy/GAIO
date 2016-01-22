% GAIO demo 
% almost invariant sets in the standard map

%% standard map
a = 0.971635;
f = @(x) mod([ x(:,1) + x(:,2) + a*sin(x(:,1)), x(:,2) + a*sin(x(:,1)) ],2*pi);
     
%% tree
n = 10; x = linspace(-1,1,n)'; [XX,YY] = meshgrid(x,x);
X = [ XX(:) YY(:) ];                     % sample points (uniform grid)
c = [pi pi]; r = [pi pi]; 
tree = Tree(c, r);                       % the box collection
sd = 8; depth = 14; 
for i = 1:depth,
    tree.set_flags('all', sd);           % flag all leaves
    tree.subdivide;                      % subdivide all flagged leaves
end

%% Ulam matrix
P = tpmatrix(tree, f, X, depth, 1);  
[xh,lambda] = eigs(P,4,'LR');
l = diag(lambda)

%% plot of second eigenvector
clf; ev = xh(:,3)/norm(xh(:,3),inf);
boxplot2(tree,'depth',depth,'density',ev); 
axis tight; axis square; shading flat;colorbar;  %caxis([-1,1]);

