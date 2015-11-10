% GAIO demo 
% global stable manifold of the origin in the Lorenz system
% -- fast version --
    
s = 10; rh = 28; b = 8/3;                       % Lorenz system
v = @(x) [s*(x(:,2)-x(:,1)) ...
          rh*x(:,1)-x(:,2)-x(:,1).*x(:,3) ...
          x(:,1).*x(:,2)-b*x(:,3)];
h = -0.01; n = 10; f = @(x) rk4(v,x,h,n);        % flow map (backward)
n = 20; X1 = linspace(-1,1,n)'; E = ones(size(X1));    
X = [ X1 -E -E; X1 -E E; X1 E -E; X1 E E; ...
      -E X1 -E; -E X1 E; E X1 -E; E X1 E; ...
      -E -E X1; -E E X1; E -E X1; E E X1];      % sample points 
c = [0 0 0]; r = [120 120 160]; 
tree = Tree(c,r);                               % the box collection

%% continuation algorithm
x = [0 0 0];      % equilibrium  
depth = 21; 
tree.insert(x', depth);
d = 3; 

none = 0; hit = 1; ins = 2; expd = 4; 
n0 = 0; n1 = tree.count(depth); 
while n1 > n0                                   % while boxes are being added
  tree.change_flags('all', ins, expd);          % flag inserted boxes for expansion
  b = tree.boxes(depth);                           
  flags = b(2*d+1,:); 
  I = find(bitand(flags,expd));                 % find boxes to be expanded  
  c = b(1:d,I); r = b(d+1:2*d,1);               % get center and radii
  n = length(I); E = ones(n,1);
  P = kron(E,X)*diag(r) + kron(c',ones(size(X,1),1)); % sample points
  tree.insert(f(P)', depth, ins, none);         % insert boxes which are hit by f(P)
  tree.unset_flags('all', expd);                % unflag recently expanded boxes
  n0 = n1; n1 = tree.count(depth); 
  disp(sprintf('%d boxes', n1));
  clf; boxplot3(tree,'alpha',0.9); axis tight;
  light('position',[-1,2,2]); view(20,30); drawnow;
end


%% cleanup
delete(tree);