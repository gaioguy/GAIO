function N = nbhd_graph(tree, depth)

% N = nbhd_graph(T, D)
%   computes a sparse matrix N of dimension b, where b is the number of
%   boxes on depth D of the tree T such that if two boxes B_i, B_j
%   intersect (at the boundary) then the (i,j)-th entry of N is nonzero

d = tree.dim;
no_of_points = 3^d;
n = tree.count(depth);
N = sparse([],[],[],n,n,n*(no_of_points-1));
grid = Points('Grid', d, no_of_points);

i = 1;
b = tree.first_box(depth);
while (~isempty(b))
  c = b(1:d);
  % this is kind of ad hoc
  r = b(d+1:2*d) + 1e-8*b(d+1:2*d);
  p = grid.get(c, r);
  v = tree.search(p, depth);
  I = find(v > 0);
  N(i,v(I)) = 1;
  i = i + 1;
  b = tree.next_box(depth);
end

delete(grid);