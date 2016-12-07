function p = macpoly(n,x)

p=0;
for k = 0:n
    p = p+(x.^k)/factorial(k);
end    
