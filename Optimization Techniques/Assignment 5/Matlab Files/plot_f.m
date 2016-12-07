function plot_f(f)

x1 = [-40:5:40];
x2 = x1;

values = zeros(length(x1),length(x2));


for i=1:length(x1)
    for j=1:length(x2)
        values(i,j) = f([x1(i) x2(j)]);
    end
end

figure(1)
surf(x1,x2,values)
title('f function Graph')
xlabel('x1')
ylabel('x2')
zlabel('f')

figure(2)
contour(x1,x2,values,'ShowText','on')
title('Contour lines of f')
xlabel('x1')
ylabel('x2')

hold off

end
