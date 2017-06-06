function A = dpgraph(t, f, X, U, depth)

% DPGRAPH dynamic programming graph
%   dpgraph(t, f, X, U, depth) computes a graph of transition 
%   costs between the boxes at level depth of the tree t using
%   the map f with sample points X and inputs U
%
%   inputs:
%   t    GAIO tree
%   f    map, f(X,U) maps mxd-matrix X and mxp-matrix U to
%        a mx(d+1)-matrix, the additional column contains 
%        the transition costs
%   X    m x d-matrix of sample points
%   U    m x p-matrix of control inputs (one for each sample point)

dim = t.dim; n = t.count(depth);
ijs = zeros(n,3);
E = ones(size(U,1),1); D = ones(size(X,1),1);
no = 1; k = 1;
b = t.first_box(depth); fprintf('%d boxes, at box no %7d',n,1);
while (~isempty(b))
  c = b(1:dim); r = b(dim+1:2*dim);
  P = X*diag(r) + ones(size(X))*diag(c);
  fP = f(kron(E,P), kron(U,D))'; 
  bn = t.search(fP(1:dim,:), depth);
  Ibn = find(bn > -1);
  [B,I,J] = unique(bn(Ibn)); 
  if k+length(B) > size(ijs,1), 
      ijs = [ijs; zeros(n,3)]; 
  end
  for l=0:length(B)-1,
    ijs(k+l,3) = min(fP(dim+1,Ibn(find(J==(l+1)))));
  end
  ijs(k:k+length(B)-1,1:2) = [B ones(length(B),1)*no];
  k = k+length(B);
  if ~rem(no,100), fprintf('\b\b\b\b\b\b\b%7d',no); end
  no = no + 1; 
  b = t.next_box(depth); 
end
A = sparse(ijs(1:k-1,1), ijs(1:k-1,2), ijs(1:k-1,3), n, n);
fprintf('\n');
