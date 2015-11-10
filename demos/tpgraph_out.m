function T = tpgraph_out(t, f, X, d)

% TPGRAPH   transition probability graph.
%   computes the graph of transition probabilities on depth d of tree t
%   using the rows of the (m x d)-matrix X as sample points and the
%   vertices of a box as image points

dim = t.dim;
N = t.count(d);
ijs = zeros(N, 3);
k = 1; no = 1;
b = t.first_box(d);
while (~isempty(b))
  c = b(1:dim); r = b(dim+1:2*dim);
  P = X*diag(r) + ones(size(X))*diag(c);
  FP = f(P)'; 
  FP = [FP-[r(1);0]*ones(1,size(FP,2)) FP+[r(1);0]*ones(1,size(FP,2)) ...
        FP-[0;r(2)]*ones(1,size(FP,2)) FP+[0;r(2)]*ones(1,size(FP,2))];
  I = t.search(FP, d);
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
end
T = sparse(ijs(1:k-1,1), ijs(1:k-1,2), ijs(1:k-1,3), N, N);
cs = sum(T);
T = T*spdiags(1./cs', 0, N, N);



