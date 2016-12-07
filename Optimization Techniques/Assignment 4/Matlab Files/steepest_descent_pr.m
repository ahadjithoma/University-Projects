function [xy]=steepest_descent_pr(x,y,e,g,s)

k=1;
xy(:,k)=[x y];
x_y(:,k)=[0 0];


while (norm( xy(:,k)-Pr(xy(:,k)-s*grad(xy(1,k),xy(2,k)))) >= e )
        
    x_y(:,k) = Pr(xy(:,k)-s*grad(xy(1,k),xy(2,k)));
    
    xy(:,k+1) = xy(:,k)+g*(x_y(:,k)-xy(:,k));
    
    if k==30000
        break
    end
    k=k+1;
    
end
create_plot_pr(xy(1,:),xy(2,:), k, g, s, e)
end