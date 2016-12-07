syms x
f=4*x^4+6*x^3 -2*x^2 -5*x +3
g=x^2+4*x+2

disp('ERWTIMA 1')
h = 5*f-3*g
solve(h,x)

disp('ERWTIMA 2')
diff(f*g,x)

disp('ERWTIMA 3')
h=f/g