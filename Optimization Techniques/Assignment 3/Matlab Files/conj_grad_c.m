function [x_out,y_out] = conj_grad_c(x,y,e)

k=1;
x(k)=x;
y(k)=y;
g = 0.5;
d = -gradf(x(k),y(k));
x(k+1) = x(k) + g*d(1);
y(k+1) = y(k) + g*d(2);

oldGrad = -d;
newGrad = gradf(x(k+1),y(k+1));

while ((norm(newGrad)) >= e)
    
    b = newGrad'*(newGrad-oldGrad) / (oldGrad'*oldGrad);
    d = - newGrad + b * (-oldGrad);
    
    if  k>5 && (f(x(k-5),y(k-5))*0.9 <= f(x(k),y(k)))
        g = g*1.5;
    elseif  k>4 && (f(x(k-4),y(k-4))*0.9 <= f(x(k),y(k)))
        g = g*1.4;
    elseif  k>3 && (f(x(k-3),y(k-3))*0.9 <= f(x(k),y(k)))
        g = g*1.3;
    elseif k>2 && (f(x(k-2),y(k-2))*0.9 <= f(x(k),y(k)))
        g = g*1.2;
    elseif k>1 && (f(x(k-1),y(k-1)) <= f(x(k),y(k)))
        g=g*1.05;
    else
        g=g*1.005
    end
    g
    k=k+1;
    x(k) = x(k-1) + g*d(1) ;
    y(k) = y(k-1) + g*d(2) ;
    
    newGrad = gradf(x(k),y(k));
    oldGrad = newGrad;
         
end

x_out=x(k);
y_out=y(k);
create_plot(x,y,k, 'Conjugate Gradient (c)');
end
