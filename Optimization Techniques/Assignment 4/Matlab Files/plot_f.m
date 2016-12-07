x=-20:1:20;
y=-20:1:20;
r = zeros( length(x) , length(y)) ;


for i=1:length(x)
    for j=1:length(y)
        r(i,j)=fun(x(i),y(j));
    end
end

figure(1)
surf(x,y,r);
title('Plot of the function')
xlabel('x')
ylabel('y')

figure(2)
contour(x,y,r,'ShowText','on');
title('Contour of the function')
xlabel('x')
ylabel('y')

