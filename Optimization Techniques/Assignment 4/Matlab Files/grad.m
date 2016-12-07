function [x] = grad(x1, x2)

%syms x1 x2;
%f = 1/2*x1^2+1/2*x2^2;

%gradf=gradient(f);

%x=subs(gradf,{x1},a);
%x=subs(x,{x2},b);

%Afou to grad tis sinartisis f mas epistrefei autousio to simeio (x1, x2),
%epistrefoume kateu8ian to simeio auto etsi wste na veltiosoume ti taxitita
%tou algori8mou. 

x=[x1; x2];
end