function [k,n,a,b] = bisection_alg(y,a,b,l,e)

k=1; %metritis epanalipsewn
n=0; %metritis upologismwn antikimenikis sinartisis 
a(k)=a;
b(k)=b;
while (b(k)-a(k))>=l 
    x1=((a(k)+b(k))/2)-e;
    x2=((a(k)+b(k))/2)+e;
    
    yX1=subs(y,x1);
    yX2=subs(y,x2);
    
    k=k+1;
    n=n+2;
    if yX1<yX2
        a(k)=a(k-1);
        b(k)=x2;
    elseif yX2<yX1
        a(k)=x1;
        b(k)=b(k-1);
    elseif yX1==yX2
        min = (x1+x2)/2
        return
    end
end
    
return
end
