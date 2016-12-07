function contour_f(f, x, k, name)

x1 = [-40:0.05:40];
x2 = x1;

values = zeros(length(x1),length(x2));

for i=1:length(x1)
    for j=1:length(x2)
        values(i,j) = f([x1(i) x2(j)]);
    end
end

contour(x1,x2,values,'ShowText','on')
hold


plot(x(1,:),x(2,:),'r*');
plot(x(1,:),x(2,:),'r-');
t=({ name;['(xo1,xo2) = (' num2str(x(1,1),'%.3f') ',' num2str(x(2,1),'%.3f') ')' ];...
   [' (x1, x2)   =  (' num2str(x(1,k),'%.3f') ',' num2str(x(2,k),'%.3f') ')' ];});

plot(x(1,k),x(2,k),'X');
title(t)
xlabel('x1')
ylabel('x2')

hold off

end
