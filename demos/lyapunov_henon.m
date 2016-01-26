%% GAIO demo: largest Lyapunov exponents
%

%% The Hénon map
a = 1.4; b = 0.3;                                       % parameters of the map
f = @(x) [1-a*x(:,1).^2+x(:,2)  b*x(:,1)];              % Hénon map
Df = @(x) [-2*a*x(:,1) 1; b 0];

%%
x = [1,1];                                              % initial point
N = 1e3; clear lam
lam(1) = log(norm(Df(x)));
D = Df(x);
for n = 1:N
    lam(n) = log(norm(D))/n;
    x = f(x);
    D = Df(x)*D;
end
loglog(abs(lam-lam(end)))
lam(end)

%% via spatial integration
n = 10; x = linspace(-1,1,n)'; [XX,YY] = meshgrid(x,x);
X = [ XX(:) YY(:) ];
c = [0 0]; r = [3 3];                         
t = Tree(c, r);
depth = 24; dx = prod(2*r)/2^depth
crs(t, f, X, depth);
b = t.boxes(depth); c = b(1:2,:)';
P = tpmatrix(t, f, X, depth, 0);
[v,e] = eigs(P,1);

%%
h = v/sum(v)*1/dx;
clear al; clear D; c = b(1:2,:)'; N = 14;
for k = 1:length(c), 
    D{k} = Df(c(k,:));
    al(k,1) = log(norm(D{k}));
    for n = 2:N,
      c(k,:) = f(c(k,:));
      D{k} = Df(c(k,:))*D{k};
      al(k,n) = log(norm(D{k}));
    end
end
clear a;
for n = 1:N,
    a(n) = sum(al(:,n).*h*dx)/n
end
hold off; plot(a,'o'); hold on

%% new sequence
clear at
for n = 2:N,
    at(n) = n*a(n)-(n-1)*a(n-1);
end
plot(at,'o'); plot(ones(1,N)*lam(end)','.-')
figure; semilogy(abs(at-lam(end)))











