function in = in_int(tree, S, N, depth)

% in_int(tree, S, N, depth)
%   checks, whether the set S is contained in the interior of the set N
%   (S, N vectors of box numbers)

d = tree.dim;
if d~=2, error('current implementation restricted to 2d.'); end
no_of_points = 3^d;
n = 3; x = linspace(-1,1,n)'; 
[XX,YY] = meshgrid(x,x);                 
X = [ XX(:) YY(:) ];

b = tree.boxes(depth);
Nf = zeros(size(b,2),1);
Nf(N) = 1;
S = S(:);

in = 1;
for i = 1:size(S,1),
  c = b(1:d,S(i));
  r = b(d+1:2*d, S(i)) + 0.1*b(d+1:2*d, S(i));
  p = X*diag(r) + ones(size(X))*diag(c); % sample points in current box
  s = tree.search(p', depth);
  if sum(Nf(s(find(s>0))))<(length(s)-length(s(find(s<0)))),
    in = 0;
    break;
  end
end
