function create_plot(x1,x2, k, name)

flag=0;
for i=1:k
    if (abs(x1(i))>2) || (abs(x2(i))>2) 
        flag=1;
    end
end
    
if flag==1
    x=-4:0.2:4;
    y=-4:0.2:4;
else
    x=-2:0.2:2;
    y=-2:0.2:2;
end

for i=1:length(x)
    for j=1:length(y)
        r(i,j)=f(x(i),y(j));
    end
end

contour(x,y,r);
hold;
plot(x1,x2,'r-');
plot(x1,x2,'r*');
plot(x1(length(x1)),x2(length(x2)),'b*')
t=({ name;['(xo,yo) = (' num2str(x1(1)) ',' num2str(x2(1)) ')' ];['k = ' num2str(k)] });
title(t)
ylabel('y');
xlabel('x');
end