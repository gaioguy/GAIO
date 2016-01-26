function rga(t, f, X, steps)

% RGA   relative global attractor.
%
%   RGA(t, f, X, steps) performs the subdivision algorithm for computing
%   the relative global attractor of the map f, cf. [1].
%
%   t       tree containing the box covering
%   f       map
%   X       m x d-matrix of sample points
%   steps   number of steps

%   [1] M. Dellnitz, A. Hohmann: A subdivision algorithm for the computation
%   of unstable manifolds and global attractors, Num. Math., 75,
%   pp. 293-317, 1997.
%
%   (C) 2013, djs GbR

dim = t.dim; hit = 1; sd = 8; tic;
for s = 1:steps,
    t.set_flags('all', sd);                % flag all boxes for subdivision
    t.subdivide(sd);                       % subdivide flagged boxes
    b = t.boxes(-1); N = size(b,2);        % get boxes from the leaves
    S = whos('X'); l = floor(5e7/S.bytes);
    for k = 0:floor(N/l),                  % split in chunks of size l
        K = k*l+1:min((k+1)*l,N);
        c = b(1:dim,K);                    % center ...
        r = b(dim+1:2*dim,1);              % ... and radii of the boxes
        n = size(c,2); E = ones(n,1);       
        P = kron(E,X)*diag(r) + ...        % sample points in all boxes
            kron(c',ones(size(X,1),1));
        t.set_flags(f(P)', hit);           % map points and flag hit boxes
    end
    t.remove(hit);                         % remove non-flagged boxes
    fprintf('depth %d, %d boxes, %.1f sec\n',t.depth,t.count(-1),toc);
end
