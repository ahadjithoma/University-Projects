% calc the cost of the recursive implementation given
% T(n) = (n/2)T(2) + 6(n/2) + 2T(n/2)

function [c] = T(n)

%init
global t_cost;


if n == 1
    c = 0;
elseif n == 2
    c = 4;
else
    c = ((n/2)*4) + (6*n/2) + 2*(T(n/2));
end
    t_cost=c;
end


