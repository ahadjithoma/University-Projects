function [out] = steepest_descent(point, e, gamma, r, grad_f, g, grad_g, grad_aux)

k=1;
x(:,k)=point;

aux_grad=grad_aux(x(:,k), r, grad_f, g, grad_g);


while norm(aux_grad)>= e
    k=k+1;
    x(:,k) = x(:,k-1)-gamma*aux_grad;
    aux_grad=grad_aux(x(:,k), r, grad_f, g, grad_g);
    if (k==10000)
        break;
    end    
end

out = x(:,end);
end