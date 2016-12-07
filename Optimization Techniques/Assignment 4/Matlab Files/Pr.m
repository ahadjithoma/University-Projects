function [ p ] = Pr( x )

p = zeros(2,1);

if ( x(1) <= -20 )
    p(1) = -20 ;
elseif ( x(1) >= 10 )
    p(1) = 10;
else
    p(1) = x(1);
end

if ( x(2) <= -12 )
    p(2) = -12 ;
elseif ( x(2) >= 15 )
    p(2) = 15;
else
    p(2) = x(2);
end


end

