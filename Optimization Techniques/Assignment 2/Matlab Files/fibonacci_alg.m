function [k,N,a,b]=fibonacci_alg(y,a,b,l)

n=1;
Fn=0;
while Fn<=((b(1)-a(1))/l)
      Fn=F(n)+F(n-1); %xrisi tis sinartisis F(n) i opoia epistrefei ton n-osto fibonacci arithmo
      n=n+1;
end

x1 = a(1)+(F(n-1-1)/F(n-1+1))*(b(1)-a(1)); 
x2 = a(1)+(F(n-1)/F(n-1+1))*(b(1)-a(1)); 

yX1 = subs(y,{x1});
yX2 = subs(y,{x2});
N=2;
for k=1:1:n-2
    if yX1>yX2
        a(k+1)=x1;
        b(k+1)=b(k);
        x1=x2;
        yX1=yX2;
        x2=a(k+1)+(F(n-k-1)/F(n-k))*(b(k+1)-a(k+1));
        yX2=subs(y,{x2});
    else
        a(k+1)=a(k);
        b(k+1)=x2;
        x2=x1;
        yX2=yX1;
        x1=a(k+1)+(F(n-k-2)/F(n-k))*(b(k+1)-a(k+1));
        yX1=subs(y,{x1});
    end
    N=N+1;
end
end