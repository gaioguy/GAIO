%% GAIO demo
% extraction of cyclic behaviour

alpha = -1.7;    % map from "On the approximation of complicated behaviour"
ft = @(z) exp(-(2*pi*i)/3)*((abs(z).^2+alpha).*z+0.5*conj(z).^2);
f = @(x) [real(ft(x(:,1)+i*x(:,2))) imag(ft(x(:,1)+i*x(:,2)))];
n = 10; x = linspace(-1,1,n)'; [XX,YY] = meshgrid(x,x);
X = [ XX(:) YY(:) ];                       % sample points (uniform grid)
c = [0 0]; r = [1 1]; B = Tree(c, r);      % the box collection B
depth = 14;
rga(B, f, X, depth);                       % compute attractor
P = tpmatrix(B, f, X, depth,'1');          % transition matrix 
[rho,lambda] = eigs(P,6);                  % eigenvectors 
l = diag(lambda)

%% extracting cyclic behaviour
r = 6; 
[m2,I] = max(min(abs(rho)')');      % index where none of the rhos vanishes
v = rho*diag(1./rho(I,:));          % scale by that component
u = v*[l l.^2 l.^3 l.^4 l.^5 l.^6]; % transform into desired basis

%% plot of the invariant density
w = abs(rho(:,1))/norm(rho(:,1),1);
boxplot2(B,'density',log10(w)); shading flat; c = colorbar; 
axis([-1 1 -1 1]); xlabel('Re'); ylabel('Im');

%% plot of the cyclic components
clf; 
for k = 1:r
    cols = ['r', 'b', 'g', 'c', 'k', 'm'];
    I = find(abs(u(:,k))>1e-8); b = B.boxes(depth);
    boxplot2(b(1:4,I)','color',cols(k),'edgecolor',cols(k)); 
    xlabel('Re'); ylabel('Im');
end

