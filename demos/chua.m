%% Relative global attractor of Chuas circuit 

%% The map
a = 16; b = 33; m0 = -0.2; m1 = 0.01;
v = @(x) [a*(x(:,2) - m0*x(:,1) - m1/3*x(:,1).^3), ...
          x(:,1)-x(:,2)+x(:,3),... 
          -b*x(:,2)];
% map is given by the time integrated vector field
h = 0.05; n = 5; f = @(x) rk4(v,x,h,n); 
x = [0.1 0.1 0.1];                                              % initial point
for i=1:10000, x = [x; f(x(end,:))]; end                        % iteration
plot3(x(:,1),x(:,2),x(:,3),'.'); xlabel('x'); ylabel('y');      % plot of the attractor

%% Preparations
n = 10; x = linspace(-1,1,n)'; [XX,YY,ZZ] = meshgrid(x,x,x);
X = [ XX(:) YY(:) ZZ(:) ];
% the tree
c = [0 0 0]; r = [12, 2.5, 20]; t = Tree(c,r);

%% Subdivison algorithm
dim = t.dim; hit = 1; sd = 8; depth = 15; tic
for i=1:depth,
    t.set_flags('all', sd);                             % flag all leaves for subdivision
    t.subdivide;                                        % subdivide flagged leaves
    b = t.boxes(-1); n = size(b,2);                     % get boxes from the leaves
    c = b(1:dim,:); r = b(dim+1:2*dim,1);               % centers and radii of the boxes
    E = ones(n,1);                                      
    P = kron(E,X)*diag(r) + kron(c',ones(size(X,1),1)); % sample points in all boxes
    t.set_flags(f(P)', hit);                            % map points and flag hitted boxes
    t.remove(hit);                                      % remove boxes which have not been hit
end
toc

%% Plot
clf; h = boxplot3(t); xlabel('x'); ylabel('y');          % plot of the covering

%% Clean up
delete(t);

