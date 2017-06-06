function T = tpmatrix(tree, f, X, depth, verbose)

% TPMATRIX   transition probability matrix.
%
%   TPMATRIX(t, f, X, d, v) computes the matrix T of transition
%   probabilities between the boxes in the tree t
%
%   t       tree containing the box covering 
%   f       map
%   X       m x d-matrix of sample points
%   d       depth of the tree on which the matrix is computed
%   v       verbose flag: '0' or '1' ('1' prints progress)

%   (C) 2013, djs2 GbR

d = tree.dim;
b = tree.boxes(depth);                       % get the geometry of the boxes
N = size(b,2); S = whos('X'); l = floor(5e7/S.bytes);
I = []; IJS = []; tic;
for k = 0:floor(N/l),                     % split in chunks of size l
    K = k*l+1:min((k+1)*l,N);
    c = b(1:d,K); r = b(d+1:2*d,1);       % center and radii of the boxes
    n = size(c,2); E = ones(n,1);         % sample points in all boxes
    P = kron(E,X)*diag(r) + kron(c',ones(size(X,1),1));
    I = tree.search(f(P)', depth);    % get box numbers of image points
    pI = find(I>0);              % I <= 0 iff f(P) is outside of covering
    J = kron(K',ones(size(X,1),1));  % column numbers
    [I,J,S] = find(sparse(I(pI), J(pI), 1, N, N));   % transition matrix
    IJS = [IJS; I,J,S];
    if verbose, fprintf('%d of %d boxes, %.1f sec\n',min((k+1)*l,N),N,toc); end
end 
T = sparse(IJS(:,1), IJS(:,2), IJS(:,3), N, N);   % transition matrix
cs = sum(T);
T = T*spdiags(1./cs', 0, N, N);      % normalize
if verbose, fprintf('\n'); end


