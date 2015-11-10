%% Relative global attractor of the Hénon map
% Oliver Junge, June 2013

%% The map
% The Hénon map $f(x,y)=(1-ax^2+y,bx)$ 
% is a well known example of a map which exhibits _complicated dynamics_ .
% Here is a plot of its attractor for $a=1.4$ and $b=0.3$.
a = 1.4; b = 0.3;                                       % parameters of the map
f = @(x) [1-a*x(:,1).^2+x(:,2)  b*x(:,1)];              % Hénon map
x = [0.1 0.1];                                          % initial point
for i=1:10000, x = [x; f(x(end,:))]; end                % iteration
plot(x(:,1),x(:,2),'.'); xlabel('x'); ylabel('y');      % plot of the attractor
axis([-1.5 1.5 -0.5 0.5])

%% Preparations
% In order to compute a covering of the attractor, we first choose sample points 
% in the square $[-1,1]^2$.  Here, we chose these points on the edges of
% the square.
n = 20; X1 = linspace(-1,1,n)'; E = ones(size(X1));     % 1d points
X = [ X1 -E; X1 E; -E X1; E X1];                        % sample points in $[-1,1]^2$
plot(X(:,1),X(:,2),'o');   xlabel('x'); ylabel('y');    % plot of the sample points
axis([-2 2 -2 2])                             
%%
% and initialize the tree using the square $[-3,3]^2$
c = [0 0]; r = [3 3];                         
t = Tree(c, r);

%% Subdivison algorithm
% We can now run the subdivision algorithm for the (relative) global
% attractor. 
dim = t.dim; hit = 1; sd = 8; depth = 22; tic
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
clf; h = boxplot2(t); xlabel('x'); ylabel('y');          % plot of the covering
axis([-1.5 1.5 -0.5 0.5]); set(gcf,'renderer','painters');

%% Clean up
delete(t);

