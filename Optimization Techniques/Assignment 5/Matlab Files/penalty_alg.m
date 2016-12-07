function [out] = penalty_alg(xo , e, f, grad_f, g, grad_g)
%penalty algorithm

k=1;
x(:,k)=xo;
r(k) = 3;
rmsDiff=0;
diff=0;

while(1)    
    k=k+1;
    x(:,k) = steepest_descent(x(:,k-1), e, 10^-5, r(k-1), grad_f, g, grad_g, @penalty_grad_f);    
        
    if (k>10000)
        break;
    end
    
    r(k) = 2 * r(k-1);
    diff = x(:,k) - x(:,k-1) ;    
    rmsDiff = sqrt(rmsDiff^2+diff'*diff);
    
    if ( (rmsDiff <= e) || (norm(diff)<=e))
        break;
    end
end
out=x;
contour_f(f, x, k, 'Penalty Method' )
end