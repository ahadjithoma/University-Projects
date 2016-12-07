function create_plot(x1,x2, k, g, name)

x=-6:0.5:6;
y=-6:0.5:6;


for i=1:length(x)
    for j=1:length(y)
        r(i,j)=fun(x(i),y(j));
    end
end

contour(x,y,r);
hold;
plot(x1,x2,'r-');
plot(x1,x2,'r*');
plot(x1(length(x1)),x2(length(x2)),'b*')

t=({ name;['(xo1,xo2) = (' num2str(x1(1),'%.3f') ',' num2str(x2(1),'%.3f') ')' ];...
   ['(x1, x2)  =  (' num2str(x1(k),'%.3f') ',' num2str(x2(k),'%.3f') ')' ]; ['g = '  num2str(g) ', k = ' num2str(k)]});
title(t)
ylabel('y');
xlabel('x');
end