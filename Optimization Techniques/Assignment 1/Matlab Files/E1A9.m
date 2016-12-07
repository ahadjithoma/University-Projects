syms x y
f = [sin(x*y);x*x+y*y;3*x-2*y]

jacobian(f,[x,y])