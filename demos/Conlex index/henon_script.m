% The following computations correspond to the examples given in Chapter 4 of
% "A rigorous numerical method in infinite dimensions" by Sarah Day, PhD dissertation
%  currently available at http://www.math.wm.edu/~sday/henon_comp.html
%  The software packages Matlab, GAIO, and homcubes are all required.
%%  Last updated 7/26/09.
%% OJ: adpated to GAIO 3.0

% Add path for GAIO, files.
addpath(genpath('/Users/junge/Dropbox/GAIO-3.0'))
addpath main

a = 1.3; b = 0.2;                                       % parameters of the map
f = @(x) [1-a*x(:,1).^2+x(:,2)/5  5*b*x(:,1)];              % Hénon map

%% An initial simulation starting at zero.
figure(1); clf
o = zeros(1,2);
for k = 1:10000, o = [o; f(o(end,:))]; end                % iteration
plot(o(:,1),o(:,2),'.'); xlabel('x'); ylabel('y');      % plot of the attractor

%% Initializing the tree in GAIO.   
K_0 = 10; K_1 = 1000;
mn = min(o(K_0:K_1,:));
mx = max(o(K_0:K_1,:));
c = (mn+mx)/2; r = 1.1*(mx-mn)/2;
tree = Tree(c, r);
  
%% Interval arithmetic is required for rigorous computations.  The following is non-rigorous.
% construct grid of sample points
n = 10; x = linspace(-1,1,n)'; 
[XX,YY] = meshgrid(x,x);                 
X = [ XX(:) YY(:) ];

%% Subdividing boxes in tree to depth of 14 
% (keeping only a covering of the maximal invariant set). 
depth = 14;
mis(tree, f, X, depth);
figure(2); clf
boxplot2(tree); xlabel('x'); ylabel('y');
   
%% Subdividing 2 more times, once in each coordinate direction 
% (only for example from Section 4.3).
depth = depth+2; sd = 8;
for i=1:2
  tree.set_flags('all',sd);
  tree.subdivide(sd);
end

%% Computing the map (sparse transition matrix) on boxes in the tree.   
P = tpgraph_out(tree, f, X, depth);
P = spones(P);
   
%% Searching for candidates for periodic orbits.
% Period 1 boxes.
p1 = find(diag(P));

% Period 2 boxes.
p2 = find(diag(P^2));
p2 = setdiff(p2, p1);
period2 = cycle(tree, p2(1), P, 2);

% Period 3 boxes.
p3 = find(diag(P^3));
p3 = setdiff(p3, p1);

% Period 4 boxes.
p4 = find(diag(P^4));
p4 = setdiff(p4, p1);
p4 = setdiff(p4, p2);

%% Connecting orbits period 2 and period 4.
period2 = cycle(tree, p2(1), P, 2);
period4 = cycle(tree, p4(1), P, 4);
sp24 = shortest_path(P, p2(1), p4(1));
sp42 = shortest_path(P, p4(1), p2(1));
extra = union(period2, period4);
S24 = union(sp24, extra);
S42 = union(sp42, extra);
   
%% Connecting orbits fixed point and period 2.
sp12 = shortest_path(P, p1(1), p2(1));
sp21 = shortest_path(P, p2(1), p1(1));
period2 = cycle(tree, p2(1), P, 2);
S12 = union(sp12, period2);
S21 = union(sp21, period2);


%% Choose ONE of the following sets to study.
S = period2;		% For example in Section 4.1.
% S = period4;
% S = S24;		% For example in Section 4.2.
% S = S42;
% S = union(S24, S42);	% For example in Section 4.3.
% S = S21;		
   
 %% Growing an isolating neighborhood for S.
I = grow_isolated(S, P, tree, depth);
    
%% Constructing an index pair (X, A).
[X, A] = build_ip(tree, depth, I, P);
   
%% Show index pair
figure(3); clf
b = tree.boxes(depth); 
boxplot2(b(1:4,X)','color','y');
boxplot2(b(1:4,A)','color','r');
  
%% Finding the image, [Y, B], of [X, A] under P.
n = tree.count(depth);
FX = find(P*flags(X,n));
Y = union(X,FX);
B = setdiff(Y,I);

%% Save sets, map to files.
b = tree.boxes(depth);
box2cub(b(:,X),'henon-X.dat')
box2cub(b(:,A),'henon-A.dat')
box2cub(b(:,Y),'henon-Y.dat')
box2cub(b(:,B),'henon-B.dat')
tm2map(tree, depth, X, Y, P, 'henon.map');

%% Compute homology using homcubes.
!homcubes -i henon.map henon-X.dat henon-A.dat henon-Y.dat henon-B.dat -R

%% or add a path ...
%% !~/../../Applications/homology/bin/./homcubes -i henon.map henon-X.dat henon-A.dat henon-Y.dat henon-B.dat -R



