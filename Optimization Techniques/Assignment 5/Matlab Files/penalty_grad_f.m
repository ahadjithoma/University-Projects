function [out] = penalty_grad_f(x, r, grad_f, g, grad_g)
%gradient of the penalty  function

out =  grad_f(x) + r * penalty_grad_abs(x , g , grad_g);

end