function X = rk4(v,X,h,n)

% RK4   Runge-Kutta scheme of order 4 
%   performs n steps of the scheme for the vector field v
%   using stepsize h on each row of the matrix X
%   v maps an (m x d)-matrix to an (m x d)-matrix 

for i = 1:n
    k1 = v(X);
    k2 = v(X + h/2*k1);
    k3 = v(X + h/2*k2);
    k4 = v(X + h*k3);
    X = X + h*(k1 + 2*k2 + 2*k3 + k4)/6;
end