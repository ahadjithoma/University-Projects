function [x_out,y_out]=steepest_descent(x,y,e,g)


k=1;
x(k)=x;
y(k)=y;

calc=norm(grad(x(k),y(k)));

while calc>= e
    
    t=grad(x(k),y(k));
    
    x(k+1)=x(k)-g*t(1);
    y(k+1)=y(k)-g*t(2);
    
    calc=norm(grad(x(k+1),y(k+1)));
    if calc<e
        break
    elseif k==30000
        break
    end
    k=k+1;      
end
x_out=x;
y_out=y;

create_plot(x,y,k,g, 'Steepest Descent');

end