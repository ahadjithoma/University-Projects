% Recursive implementation of the FFT

function [y] = recursiveFFT(x)

% init
n = length(x);      % n is power of 2    
k = 0:n/2-1;        % get k 
global rec_cost;    % for calculating the cost of the algorithm 

if n == 1 
    y = x;
else 
    
    % recursive call
    y0 = recursiveFFT( x(1:2:(n-1)) );
    y1 = recursiveFFT( x(2:2:n) );
    
    % calc Wn
    wn = exp(-2*pi*1i/n).^k;
    
    % calc Im{}
    z = wn.*y1;
    
    % return
    y = [y0 + z , y0 - z];
    
    % calc cost
    rec_cost = rec_cost+6*n/2;
    rec_cost = rec_cost+n; 
    rec_cost = rec_cost+4; 
end


end

%% Helpful Sites for the implementation
%
%   https://jakevdp.github.io/blog/2013/08/28/understanding-the-fft/
%   http://stackoverflow.com/questions/28009590/understanding-the-fft-recursive-algorithm
%
% Recursive Implamentation is based on the second link
%
