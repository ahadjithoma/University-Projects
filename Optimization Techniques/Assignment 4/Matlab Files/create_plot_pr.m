function create_plot_pr(x1,x2, k, g, s, e)

x=-12:0.5:12;
y=-12:0.5:12;


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
name = 'Steepest Descent with Projection';
t=({ name;['(xo1,xo2) = (' num2str(x1(1),'%.4f') ',' num2str(x2(1),'%.4f') ')' ];...
   ['(x1, x2)  =  (' num2str(x1(k),'%.4f') ',' num2str(x2(k),'%.4f') ')' ];...
   ['g = '  num2str(g) ', s = ' num2str(s) ', e = ' num2str(e) ', k = ' num2str(k)]});
title(t)
ylabel('y');
xlabel('x');
end