% calculate costs using T.m and recursiveFFT.m  
% display and plot results 

% init
global rec_cost;
global t_cost;
n = 8; % set the max 2^n power

% allocate
T_cost = zeros(n,1);
recursive_cost = zeros(n,1);

for i=1:n
    
    % set cost to zero
    rec_cost=0; 
    t_cost=0;
    
    % signal generation
    x = (cos(4*pi*(1:2^i)/8+sin(2*pi*(1:2^i)/8)));
    
    % run algs
    recursiveFFT(x);
    T(2.^i);
    
    % get costs
    recursive_cost(i) = rec_cost;
    T_cost(i) = t_cost;
end


% display results
display(T_cost);
display(recursive_cost);

% plot
figure;
semilogy(1:n,T_cost,1:n,recursive_cost)
title('Algorithms costs');
xlabel('power of 2');
ylabel('flops');


