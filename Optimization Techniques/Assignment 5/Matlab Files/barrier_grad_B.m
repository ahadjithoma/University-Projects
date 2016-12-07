function [out] = barrier_grad_B(x, g, grad_g)
%gradinet of B function at x

sum = zeros(size(2));
restrictions = g(x);
grad_restr = grad_g(x);

for i =1:length(grad_restr)
    v = grad_restr(i,:);
    val = restrictions(i);
    sum = sum + v / ( val^2 ) ;
end

out = sum' ;