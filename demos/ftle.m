global tau ;
tau = 10;
f = @(x) double_gyre_map(x);

%% constructing the box partition
c = [1 0.5]; r = [1 0.5];                         
t = Tree(c, r);
dim = t.dim; sd = 8; depth = 12;   % depth 12 means 2^12 boxes, i.e. 2^6=64 in each direction
for i=1:depth,
    t.set_flags('all', sd);        % flag all leaves for subdivision
    t.subdivide;                   % subdivide flagged leaves
end

%% computing the FTLE field
n = 20; X = 2*rand(n,2)-1;         % sample points
h = 1e-6; ftle = zeros(t.count(-1),1);
b = t.first_box(-1); k = 1; 
while (~isempty(b))
    c = b(1:dim); r = b(dim+1:2*dim);         % center and radius of current box
    p = X*diag(r) + ones(size(X))*diag(c);    % sample points in the current box
    q = X*diag(r) + ones(size(X))*diag(c)+h*(2*rand(size(X,1),2)-1);  % slightly shifted sample points
    d = sum(((f(p)-f(q)).^2)')./sum(((p-q).^2)'); % relative distances of their images
    ftle(k) = max(log(d)/(2*tau));            % the FTLE in that box
    if rem(k,100)==0 disp(sprintf('box no %d',k)); end
    b = t.next_box(-1); k = k+1;
end
toc

%% plot
clf; boxplot2(t,'density', ftle); colormap hot; shading flat

