function [out] = grad_aux_barrier(x, r, grad_f, g, grad_g)
%gradient of the auxiliary function at x

out = grad_f(x) + r*barrier_grad_B(x,g,grad_g);
end