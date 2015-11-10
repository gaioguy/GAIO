function C = conn_comp(N, S, b)

% C = conn_comp(N, S, B)
%   computes the connected component of the subset S containing the box B;
%   N is a neighborhood graph; S a vector of box numbers and B a box
%   number (1<=B<=size(N))

Cf = zeros(1,size(N,1));
A = N(S, S);
A = sparse(A);
G = Graph(A);
scc = G.strong_components;
c = zeros(1,size(N,1));
for i=1:max(scc),
  c(S(find(scc==i))) = i;
end
Cf(find(c==c(b))) = 1;
C = find(Cf);

delete(G);


