function [out] = grad_f1(x)

x1 = 4 * x(1) -3 * x(2) ;
x2 = -3 * x(1) + 4 * x(2) ;
out = [x1 x2]';

end

