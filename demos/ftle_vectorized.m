% by Adrian Ziessler, ziessler@math.upb.de, June 2016

global tau ;
tau = 10;
f = @(x) double_gyre_map(x);

%% constructing the box partition
c = [1 0.5]; r = [1 0.5];                         
t = Tree(c, r);
dim = t.dim; sd = 8; depth = 12;   % depth 12 means 2^12 boxes, i.e. 2^6=64 in each direction
for i=1:depth
    t.set_flags('all', sd);        % flag all leaves for subdivision
    t.subdivide;                   % subdivide flagged leaves
end

%% computing the FTLE field
n = 100;
try
    HS = haltonset(dim,'Skip',2);
    X = 2*net(HS,n)-1;         % sample points
catch
    X = 2*rand(n,2)-1; 
end

h = 1e-6;
b = t.boxes(-1);
S = whos('X'); l = floor(5e7/S.bytes);
N = size(b,2);
ftle = zeros(N,1); dispr('',0);
for k = 0:floor(N/l)
    K = k*l+1:min((k+1)*l,N);
    c = b(1:dim,K); r = b(dim+1:2*dim,1);         % center and radius of current box
    n = size(c,2); E = ones(n,1);    
    P = kron(E,X)*diag(r) + kron(c',ones(size(X,1),1));
    Q = kron(E,X)*diag(r) + kron(c',ones(size(X,1),1))+h*(2*rand(size(P,1),dim)-1);
    D = sum(((f(P)-f(Q)).^2)')./sum(((P-Q).^2)'); % relative distances of their images
    ftle(K,1) = max(log(reshape(D,size(X,1),n))/(2*tau))';            % the FTLE in that box
    dispr(sprintf('Step %d of %d',k+1,floor(N/l)+1),1);
end

%% plot
clf; boxplot2(t,'density', ftle); colormap hot(1024); shading flat

