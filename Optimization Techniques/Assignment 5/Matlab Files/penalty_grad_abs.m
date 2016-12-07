function [out] = penalty_grad_abs(x, g, grad_g)
%Gradient of absolute value penalty function


sum = [0 0];
restrictions = g(x);
grad_restr = grad_g(x);


for i =1:length(grad_restr)
    val = restrictions(i);
    if (val>=0)
        v = grad_restr(i,:);
        sum = sum + 2 * val * v;
    end
end

out = sum' ;

end

