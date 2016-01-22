
%% GAIO demo: Invariant measure in the Lorenz system
%

s = 10; rh = 28; b = 0.4;                % the Lorenz system
v = @(x) [s*(x(:,2)-x(:,1)) ...            
          rh*x(:,1)-x(:,2)-x(:,1).*x(:,3) ...
          x(:,1).*x(:,2)-b*x(:,3)];
h = 0.01; n = 10; f = @(x) rk4(v,x,h,n); % f is the flow

%% Preparations
% choose sample points, construct box tree
n = 7; x = linspace(-1,1,n)'; [XX,YY,ZZ] = meshgrid(x,x,x);
X = [ XX(:) YY(:) ZZ(:) ];               % $7^3$ sample points 
c = [0.1 0.1 27]; r = [30 30 40];             
t = Tree(c,r);                           % the box collection

%% Computation of the global unstable manifold
a = sqrt(b*(rh-1)); x0 = [a a rh-1;-a -a rh-1]; % equilibria
depth = 21; t.insert(x0', depth);        % construct [x0]
gum(t, f, X, depth);                     % compute global unstable manifold

%% Computation of the invariant measure
X = 2*rand(100,3)-1;                     % points for Monte Carlo quadrature
P = tpmatrix(t, f, X, depth, 1);            % transition matrix 
[w,lambda] = eigs(P,1,'lm');             % compute eigenvector at eigenvalue 1
wp = log10(max(abs(w(:,1)),1e-16));      % plot measure logarithmically

%% Visualization
boxplot3(t,'depth',depth,'density', wp ,'alpha',0.1);   
load lorenz_cmap; colormap(cmap);        % special colormap
view(20,30); axis square; axis tight;
xlabel('x'); ylabel('y'); zlabel('z'); 

%% Cleanup
delete(t);
