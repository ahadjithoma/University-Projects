function [out] = barrier_alg(xo , e, f, grad_f, g, grad_g)
%barrier algorithm

k=1;
x(:,k)=xo;
B = barrier_B(x(:,k),g);
func = f(x(:,k));
r(k) = 0.2*func/B;

rmsDiff=0;
while(1)
    
    k=k+1;
    
    x(:,k) = steepest_descent(x(:,k-1), e, 0.0001, r(k-1), grad_f, g, grad_g, @barrier_grad_aux);       
        
    if (k>10000)
        break;
    end
    
    r(k) = 0.8 * r(k-1);
    diff = x(:,k) - x(:,k-1) ;    
    rmsDiff = sqrt(rmsDiff^2+diff'*diff);
    
    if ( (rmsDiff <= e) || (norm(diff)<=e))
        break;
    end
end
out=x;
contour_f(f, x, k, 'Barrier Method' )
end