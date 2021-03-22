%% GAIO demo: global unstable manifold in the Lorenz system

% the Lorenz system
sigma = 10; rho = 28; beta = 8/3;
v = @(x) [sigma*(x(:,2)-x(:,1)) ...             % the Lorenz system
          rho*x(:,1)-x(:,2)-x(:,1).*x(:,3) ...
          x(:,1).*x(:,2)-beta*x(:,3)];
f = @(x) rk4(v,x,0.01,10);                       % flow map

% choose sample points in the cube [-1,1]^3  
n = 7; x = linspace(-1,1,n)'; 
[XX,YY,ZZ] = meshgrid(x,x,x);                 
X = [ XX(:) YY(:) ZZ(:) ];                          

% containing one of the two unstable equilibria of the Lorenz system
center = [0 0 27]; radius = [30 30 40]; 
tree = Tree(center, radius);         

% construct box which contains an equilibrium 
x0 = [sqrt(beta*(rho-1)) sqrt(beta*(rho-1)) rho-1];  % an unstable equilibrium
depth = 18;                                 
tree.insert(x0', depth);                                % construct box around x0

% compute global unstable manifold of x0 (= attractor in this case)
gum(tree, f, X, depth)

% plot of the box covering
boxplot3(tree); view(20,30); axis tight; axis square;
