function X = rk4u(v,X,U,h,n)

% RK4U   Runge-Kutta scheme of order 4 for control system
%   rk4u(v,X,U,h,n) performs n steps of the scheme for the vector field v
%   using stepsize h on each row of the matrix X
%
%   v(X,U) maps an (m x d)-matrix X and an (m x p)-matrix U
%          to an (m x d)-matrix 

for i = 1:n
    k1 = v(X,U); 
    k2 = v(X + h/2*k1,U);
    k3 = v(X + h/2*k2,U);
    k4 = v(X + h*k3,U);
    X = X + h*(k1 + 2*k2 + 2*k3 + k4)/6;
end