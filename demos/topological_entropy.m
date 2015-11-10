% GAIO 3.0 demo
% topological entropy of the Henon map

% Henon map
a = 1.4; b = 0.3;
f = @(x) [1 - a*x(:,1).^2 + b*x(:,2) x(:,1)];   
% sample points on agrid
n = 40; X1 = linspace(-1,1,n)'; 
[XX,YY] = meshgrid(X1,X1); X = [ XX(:) YY(:) ];
% the tree
c = [0 0]; r = [1.5 1.5]; t = Tree(c, r);

%% subdivision algorithm for the chain recurrent sets
dim = t.dim; hit = 1; sd = 8; depth = 16; tic
for d = 1:depth
  t.set_flags('all', sd);
  t.subdivide;
  G = tpgraph_out(t, f, X, d);  % not rigorous yet !!
  sc = scc(G,G');
  sc(find(sc > -1)) = 1; sc(find(sc == -1)) = 0;
  flags = sprintf('%1d', sc);
  t.set_flags(flags, hit);
  t.remove(hit);
  fprintf('step %d: %d boxes\r', d, t.count(-1));
end

%% computing the entropy
tic; depth = 16;
b = t.boxes(depth);
letters = zeros(size(b,2),1);
I = find(b(2,:) > 0); letters(I) = 1;
% note: tow letters suffice here (since entropy <= log(2))
G = tpgraph_out(t, f, X, depth); 
start = 2;
[i,j,e] = subshift(G, uint8(letters), start); S = sparse(i,j,e);
l = eigs(S);  log(max(l))
toc

%% clean up
delete(t);
