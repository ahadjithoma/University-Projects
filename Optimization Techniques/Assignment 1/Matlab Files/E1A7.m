syms x

y=3*x^4-16*x^3+18*x^2

rizesY = solve(y,x)

y1=diff(y,x,1)
y2=diff(y,x,2)
y3=diff(y,x,3)

a=solve(y1,x)
subs(y2,{x},a)

b=solve(y2,x)
subs(y3,{x},b)