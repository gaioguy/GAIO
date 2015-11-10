function mis(t, f, X, steps)

% MIS   maximal invariant set.
%
%   MIS(t, f, X, steps) performs the subdivision algorithm for computing
%   the maximal invariant set of the map f.
%
%   t       t containing the box covering
%   f       map
%   X       m x d-matrix of sample points
%   steps   number of steps
%
%   (C) 2015, djs GbR

d = t.dim; hit = 1; sd = 8; 
for i = 1:steps
  t.set_flags('all', sd);                % flag all boxes for subdivision
  t.subdivide(sd);                       % subdivide flagged boxes
  n = t.count(-1);
  unset = ones(1, n);
  j = 1;
  b = t.first_box(-1);
  while (~isempty(b))
    c = b(1:d); r = b(d+1:2*d);
    P = X*diag(r) + ones(size(X))*diag(c); % sample points in current box
    fP = f(P);
    unset(j) = 1-t.set_flags(fP', hit);
    j = j + 1;
    b = t.next_box(-1);
  end
  flags = sprintf('%1d', unset);
  t.unset_flags(flags, hit);
  t.remove(hit);
  disp(sprintf('step %d: %d boxes', i, t.count(-1)));
end

