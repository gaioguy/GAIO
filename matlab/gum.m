function gum(T, f, X, depth)

% GUM   global unstable manifold.
%
%   GUM(t, f, X, steps) performs the continuation algorithm for computing
%   (part of) some global unstable manifold of the map f, cf. [1].
%
%   t       tree containing the box covering (and initially a covering
%           of the object for which the unstable manifold is to be computed)
%   f       map
%   X       m x d-matrix of sample points
%   depth   depth of the tree on which the continuation is performed

%   [1] M. Dellnitz, A. Hohmann: The computation of unstable manifolds
%   using subdivision and continuation, in: H.W. Broer and S.A. van Gils
%   and I. Hoveijn and F. Takens (eds.), Nonlinear Dynamical Systems and
%   Chaos, Birkh\"auser, PNLDE 19, pp. 449-459, 1996.
%
%   (C) 2013, djs GbR

dim = T.dim;
none = 0; ins = 2; expd = 4;         % defining flags
nb0 = 0;  nb1 = T.count(depth);      % bookkeeping of the number of boxes
tic; j = 1;
while nb1 > nb0                      % while new boxes are being inserted
  T.change_flags('all', ins, expd);  % mark inserted boxes as to be expanded
  b = T.boxes(depth);                % get the geometry of the boxes
  flags = b(7,:); 
  I = find(bitand(flags,expd));          % find boxes to expand
  b = b(:,I); N = size(b,2);
  S = whos('X'); l = floor(5e7/S.bytes); dispr('',0);
  for k = 0:floor(N/l),                  % split in chunks of size l
      K = k*l+1:min((k+1)*l,N);
      c = b(1:dim,K);                    % center ...
      r = b(dim+1:2*dim,1);              % ... and radii of the boxes
      n = size(c,2); E = ones(n,1);
      P = kron(E,X)*diag(r) + ...        % sample points in all boxes
          kron(c',ones(size(X,1),1));
      T.insert(f(P)', depth, ins, none); % map sample points and insert boxes
      dispr(sprintf('step %d: %d of %d boxes, %.1f sec',j,min((k+1)*l,N),N,toc),1);
  end
  fprintf('\n');
  T.unset_flags('all', expd);        % unflag recently expanded boxes
  nb0 = nb1; nb1 = T.count(depth);
  j = j+1;
end
