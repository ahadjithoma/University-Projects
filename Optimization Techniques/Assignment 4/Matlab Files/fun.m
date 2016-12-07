function [x] = fun(x1, x2)

%syms x1 x2;
%f = 1/2*x1^2+1/2*x2^2;
x=[x1 x2];
x = 1 / 2 * sum( x.^2 ) ;

end