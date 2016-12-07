function [g] = BisectDer(a,b,x,y,l,d)

k=1;
a(k)=a;
b(k)=b;
xy=[x;y];

while (b(k)-a(k)>l)
    
    xy2(k) = (a(k)+b(k))/2;
    
    xyt = xy + xy2(k) * d ;
    
    df =  gradf(xyt(1),xyt(2))' * d ; 
    
    if df>0
        a(k+1)=a(k);
        b(k+1)=xy2(k);
    elseif df<0
        a(k+1)=xy2(k);
        b(k+1)=b(k);
    else 
        break;
    end
    k=k+1;
end

g = 1/2 * ( a(length(a)) + b(length(b)));

end