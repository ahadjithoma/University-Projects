syms x1 x2

y = 30*x1 + 16*x2 - x1^2 + x1*x2 - 2*x2^2

disp('ERWTIMA 1')
y1=diff(y,x1)
y2=diff(y,x2)

disp('ERWTIMA 2')
GradY=jacobian(y,[x1,x2])

disp('ERWTIMA 3')
HessianY=hessian(y,[x1,x2])
