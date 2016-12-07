function [out] = barrier_B(x, g)
%B function at x

sum=0;

restrictions = g(x);

for i =1:length(g)
    sum = sum - 1 / restrictions(i);
end

out = sum; 
    
end