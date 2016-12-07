function [k,n,a,b] =  golden_section_alg(y,a,b,l)

g = 0.618;
k=1;
n=2;
x1 = a(k) + (1-g)*(b(k)-a(k));
x2 = a(k) + g*(b(k)-a(k));

yX1 = subs(y,{x1});
yX2 = subs(y,{x2});

while (b(k)-a)>=l
    if yX1>yX2
        a(k+1)=x1;
        b(k+1)=b(k);
        x1=x2;
        yX1=yX2;
        x2=a(k+1)+g*(b(k+1)-a(k+1));
        yX2=subs(y,{x2});
        n=n+1;
    elseif yX2>yX1
        a(k+1)=a(k);
        b(k+1)=x2;
        x2=x1;
        yX2=yX1;
        x1=a(k+1)+(1-g)*(b(k+1)-a(k+1));
        yX1=subs(y,{x1});
        n=n+1;
    elseif yX1==yX2
        min = (x1+x2)/2
        return
    end
    k=k+1;
end

end