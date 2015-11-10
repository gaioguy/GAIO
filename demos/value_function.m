%% value function for an inverted pendulum on a cart

m = 2; M = 8; l = 0.5; g = 9.8; q1 = 0.1; q2 = 0.05; r0 = 0.01; m_r = m/(m+M);
v = @(x,u) [ x(:,2), ...                            % vector field of the pedulum
             (g/l*sin(x(:,1)) - 0.5*m_r*x(:,2).^2.*sin(2*x(:,1)) - ...
             m_r/(m*l)*u.*cos(x(:,1)))./(4.0/3.0 - m_r*cos(x(:,1)).^2), ...
             0.5*( q1*(x(:,1).^2) + q2*(x(:,2).^2) + r0*u.^2 )]; 
h = 0.02; steps = 5;                                    % step size and # of steps
f = @(x,u) rk4u(v,[x zeros(size(x,1),1)],u,h,steps);    % control system
n = 2; x = linspace(-1,1,n)'; [XX,YY] = meshgrid(x,x); 
X = [ XX(:) YY(:) ];                                % sample points
U = linspace(-128,128,33)';                           % control samples

depth = 16; c = [1e-5 1e-5]; r = [8 10]; 
tree = Tree(c, r);                                  % construct full tree
for i = 1:depth,                                    
    tree.set_flags('all', 8);  tree.subdivide; 
end
A = dpgraph(tree, f, X, U, depth);                   % compute graph
dest = tree.search([0;0], depth);                    % find destination box
[V,~] = dijkstra(A', dest);                          % compute value function

%%
V(find(V < 0)) = Inf;
clf; boxplot2(tree, 'density', max(min(V,7),0));     % plot value function
colorbar; colormap jet; shading flat; axis('tight')

