function N = get_nbhd(tree, S, depth)

% N = get_nbhd(tree, S, depth)
%   computes the smallest 'representable' neighbourhood of the given subset
%   S of the boxes on depth D of the tree T; S is a vector of box numbers

% see o.m also
  
d = tree.dim;
b = tree.boxes(depth);
S = S(:);
n = zeros(1, size(b,2));

for bi = 1:size(S,1),
  c = b(1:d, S(bi));
  r = b(d+1:2*d, S(bi)) + 0.1*b(d+1:2*d, S(bi));
  I = tree.search_box(c, r);
  n(I) = 1;
end

N = find(n);
