% GAIO demo
% chain recurrent set for the Henon map

% Henon map
a = 1.3; b = 0.2;
f = @(x) [1-a*x(:,1).^2+x(:,2)./5 b*x(:,1)*5];   
% sample points on the edges of the unit square
n = 40; X1 = linspace(-1,1,n)'; 
X = [ X1 -ones(size(X1)); X1 ones(size(X1)); -ones(size(X1)) X1; ones(size(X1)) X1];    
% the tree
c = [0 0]; r = [1.5 1.5]; t = Tree(c, r);

%% subdivision algorithm
dim = t.dim; hit = 1; sd = 8; depth = 14; tic
for d = 1:depth
  t.set_flags('all', sd);
  t.subdivide;
  G = tpmatrix(t, f, X, d, 0);
  sc = scc(G,G');
  sc(find(sc > -1)) = 1; sc(find(sc == -1)) = 0;
  flags = sprintf('%1d', sc);
  t.set_flags(flags, hit);
  t.remove(hit);
  fprintf('step %d: %d boxes\r', d, t.count(-1));
end

%% plot
boxplot2(t); xlabel('$x$'); ylabel('$y$');

%% clean up
delete(t);