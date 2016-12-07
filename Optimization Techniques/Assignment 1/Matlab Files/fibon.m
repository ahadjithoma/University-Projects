function y = fibon(a0,a1,n)
y = 1:n;
y(1)= a0;
y(2)= a1;
for i = 3:n
    y(i)= y(i-1)+y(i-2);
end
