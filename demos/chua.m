%% GAIO demo
% almost invariant sets in Chua's circuit 

%% the map
a = 16; b = 33; m0 = -0.2; m1 = 0.01;            % Chua's circuit
v = @(x) [a*(x(:,2) - m0*x(:,1) - m1/3*x(:,1).^3), ...
          x(:,1)-x(:,2)+x(:,3),... 
          -b*x(:,2)];
h = 0.05; n = 5; f = @(x) rk4(v,x,h,n);          % f is the flow map

%% preparations
n = 5; x = linspace(-1,1,n)'; [XX,YY,ZZ] = meshgrid(x,x,x);
X = [ XX(:) YY(:) ZZ(:) ];                       % sample points
c = [0 0 0]; r = [12, 2.5, 20]; t = Tree(c,r);   % the box collection

%% computing the covering (here, by continuation)
x0 = [sqrt(3*m0/m1) 0 -sqrt(3*m0/m1)];           % equilibrium
depth = 24; t.insert([x0; -x0]', depth);         % initialization
gum(t, f, X, depth);                             % global unstable manifold

%% computing the measure
X = 2*rand(100,3)-1;                             % points for MC quadrature
P = tpmatrix(t, f, X, depth, 1);                 % transition matrix 
[w,lambda] = eigs(P,2,'lr',opts);                % eigenvectors

%% plot
% wp = log10(abs(w(:,1)));                         % uncomment for inv. measure
wp = log10(max(w(:,2),1e-16))-log10(max(-w(:,2),1e-16)); % 2nd eigenvector 
clf; boxplot3(t,'depth',depth,'density', wp,'alpha',0.1);   
load lorenz_cmap; colormap(cmap); colorbar       % special colormap
view(20,20); axis square; axis tight;
xlabel('x'); ylabel('y'); zlabel('z'); 
