%% GAIO demo: largest Lyapunov exponents
%
mu = 4; f = @(x) mu*x.*(1-x);          % the logistic map
df = @(x) mu*(1-2*x);

%% compute Lyapunov exponent
x = 0.95;
N = 1e5; clear lam
lam(1) = log(abs(df(x)));
for n = 1:N
    x = f(x);
    lam(n+1) = 1/(n+1)*(n*lam(n) + log(abs(df(x))));
end
%loglog(abs(lam))
lam(end)

%% Lyapunov exponent via spatial integration
n = 10; X = linspace(-1,1,n)';        % uniform grid of sample points 
c = [0.5]; r = [0.5]; t = Tree(c, r);  % the tree
sd = 8; depth = 18; dx = 1/2^depth;
for i = 1:depth,
    t.set_flags('all', sd);
    t.subdivide;
end
b = t.boxes(depth); c = b(1,:);
P = tpmatrix(t, f, X, depth, 0);
[v,e] = eigs(P,1); e
%%
h = v/sum(v)*1/dx; 
lambda = sum(log(abs(df(c))).*h'*dx)







