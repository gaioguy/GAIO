function T = tpgraph(t, f, X, d)

% TPGRAPH   transition probability graph.
%
%   TPGRAPH(T, f, X, d) computes the graph of transition
%   probabilities under the map f on the box collection
%   defined by the nodes on depth d of tree T using the
%   rows of the (m x d)-matrix X as sample points

dim = t.dim;
N = t.count(d); 
ijs = zeros(N, 3); 
k = 1; no = 1;
b = t.first_box(d);
fprintf('tpgraph: %3d%%',0);
while (~isempty(b))
  c = b(1:dim); r = b(dim+1:2*dim);
  P = X*diag(r) + ones(size(X))*diag(c);
  I = t.search(f(P)', d);
  I = I(find(I>0));
  if (length(I) > 0)
    [I,J,S] = find(sparse(I, no, 1, N, N));
    if k+length(I) > length(ijs)
        ijs = [ijs; zeros(size(ijs,1),3)];
    end
    ijs(k:k+length(I)-1,:) = [I J S];
  end
  k = k + length(I);
  b = t.next_box(d);
  no = no + 1;
  if ~rem(no,floor(N/100)), 
        fprintf('\b\b\b\b%3d%%',ceil(no*100/N)); 
  end
end
T = sparse(ijs(1:k-1,1), ijs(1:k-1,2), ijs(1:k-1,3), N, N);
cs = sum(T);
T = T*spdiags(1./cs', 0, N, N);
fprintf('\n');



