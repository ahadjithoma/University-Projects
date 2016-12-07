function [out] = grad_f2( x )
% Function that calculates the gradient of g
% at x.
x1 = 2 * x(1) -2 * x(2) ;
x2 = -2 * x(1) + 2 * x(2) ;
out = [x1 x2]';

end

