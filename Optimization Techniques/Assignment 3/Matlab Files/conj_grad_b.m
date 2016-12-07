function [x_out,y_out] = conj_grad_b(x,y,e)

k=1;
x(k)=x;
y(k)=y;

d = -gradf(x(k),y(k));

g = BisectDer(0,1,x(k),y(k),e,d);

x(k+1) = x(k) + g(k)*d(1);
y(k+1) = y(k) + g(k)*d(2);

oldGrad = -d;
newGrad = gradf(x(k+1),y(k+1));

while ((norm(newGrad)) >= e)
    k=k+1;

    b = newGrad'*(newGrad-oldGrad) / (oldGrad'*oldGrad);
    d = - newGrad + b * d;
    
    g = BisectDer(0,1,x(k-1),y(k-1),e,d);
    
    x(k) = x(k-1) + g*d(1) ;
    y(k) = y(k-1) + g*d(2) ;
    
    newGrad = gradf(x(k),y(k));
    oldGrad = newGrad;
     
    
end

x_out=x(k);
y_out=y(k);
create_plot(x,y,k, 'Conjugate Gradient (b)');
end
