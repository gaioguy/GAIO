function y = double_gyre_map(x)

global tau;

A = 0.25; eps = 0.25; om = 2*pi;
n = 40; h = tau/n; t = 0;

    function y = v(x)
        
        function y = f(x,t)
            y = eps*sin(om*t).*x.^2 + (1-2*eps*sin(om*t)).*x;
        end
        
        function y = df(x,t)
            y = 2*eps*sin(om*t).*x + (1-2*eps*sin(om*t));
        end
        
        y = [-pi*A*sin(pi*f(x(:,1),x(:,3))).*cos(pi*x(:,2)) ...
            pi*A*cos(pi*f(x(:,1),x(:,3))).*sin(pi*x(:,2)).*df(x(:,1),x(:,3)) ...
            ones(size(x,1),1)];
    end

X = [x ones(size(x,1),1)*t];
z = rk4(@v,X,h,n);
y = z(:,1:2);

end




