function [k,N,a,b]=bisection_der_alg(y,a,b,l)

n=0;
while ((0.5)^n>l/(b(1)-a(1)))
    n=n+1;
end

dy = diff(y,'x');
N=0;
for k=1:n+1
    xk = (a(k)+b(k))/2;
    dyxk = subs (dy, {xk});
    N=N+1;
    if dyxk==0
        disp('min = ');
        disp(xk);
        return;
    end
    
    if dyxk>0
        a(k+1)=a(k);
        b(k+1)=xk;
    elseif dyxk<0
        a(k+1)=xk;
        b(k+1)=b(k);
    end
end
end