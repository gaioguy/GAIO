% GAIO demo 
% global unstable manifold of an equilibrium in the Lorenz system
% -- fast version --
    
s = 10; rh = 28; b = 0.4;                       % Lorenz system
v = @(x) [s*(x(:,2)-x(:,1)) ...
          rh*x(:,1)-x(:,2)-x(:,1).*x(:,3) ...
          x(:,1).*x(:,2)-b*x(:,3)];
h = 0.01; n = 20; f = @(x) rk4(v,x,h,n);        % flow map
n = 7; x = linspace(-1,1,n)'; [XX,YY,ZZ] = meshgrid(x,x,x);
X = [ XX(:) YY(:) ZZ(:) ];                      % sample points (uniform grid)
c = [0 0 27]; r = [30 30 40]; tree = Tree(c,r);    % the box collection

%% continuation algorithm
d = 3; depth = 21; 
x0 = [sqrt(b*(rh-1)) sqrt(b*(rh-1)) rh-1];      % equilibrium  
tree.insert(x0', depth);

none = 0; hit = 1; ins = 2; expd = 4; 
n0 = 0; n1 = tree.count(depth); tic
while n1 > n0                                   % while boxes are being added
  tree.change_flags('all', ins, expd);             % flag inserted boxes for expansion
  b = tree.boxes(depth);                           
  flags = b(2*d+1,:); 
  I = find(bitand(flags,expd));                 % find boxes to be expanded  
  c = b(1:d,I); r = b(d+1:2*d,1);               % get center and radii
  n = length(I); E = ones(n,1);
  P = kron(E,X)*diag(r) + kron(c',ones(size(X,1),1)); % sample points
  tree.insert(f(P)', depth, ins, none);            % insert boxes which are hit by f(P)
  tree.unset_flags('all', expd);                   % unflag recently expanded boxes
  n0 = n1; n1 = tree.count(depth); 
  disp(sprintf('%d boxes', n1));
  boxplot3(tree); view(20,30); axis square; axis tight; drawnow; pause
end
toc

%% plot
boxplot3(tree); view(20,30); axis square; axis tight;
xlabel('x'); ylabel('y'); zlabel('z'); 

%% cleanup
delete(tree);