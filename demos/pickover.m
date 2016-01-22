% GAIO demo 
% attractor of the pickover system
    
a = 1.76; b = .4; c = -.6; d = -2.4;     % pickover system
f = @(x) [sin(a*x(:,2)) - x(:,3).*cos(b*x(:,1)), ...
          x(:,3).*sin(c*x(:,1)) - cos(d*x(:,2)), ...
          sin(x(:,1))];
n = 7; x = linspace(-1,1,n)'; [XX,YY,ZZ] = meshgrid(x,x,x);
X = [ XX(:) YY(:) ZZ(:) ];                      % sample points (uniform grid)
c = [0 0 0]; r = [2 2 1]; tree = Tree(c,r);    % the box collection

N = 1e5; x = zeros(N,3);                 % simulation
for k = 1:N-1, x(k+1,:) = f(x(k,:)); end
plot3(x(:,1),x(:,2),x(:,3),'.'); 

%% Computation of the global unstable manifold
x0 = x(end,:);                           % some point on the attractor
depth = 21; tree.insert(x0', depth);     % construct [x0]
gum(tree, f, X, depth);                  % compute global unstable manifold

%% Computation of the invariant measure
X = 2*rand(100,3)-1;                     % points for Monte Carlo quadrature
P = tpmatrix(tree, f, X, depth, 1);      % transition matrix 
[w,lambda] = eigs(P,1,'lm');             % compute eigenvector at eigenvalue 1
wp = log10(max(abs(w(:,1)),1e-16));      % plot measure logarithmically

%% Visualization
boxplot3(tree,'depth',depth,'density', wp ,'alpha',0.1);   
load lorenz_cmap; colormap(cmap);        % special colormap
view(20,30); axis square; axis tight;
xlabel('x'); ylabel('y'); zlabel('z'); 

%% Cleanup
delete(tree);
