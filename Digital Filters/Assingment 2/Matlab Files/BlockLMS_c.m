% implementation of Block-LMS algorithm using FFT

clear all
close all
tic     % start timer


% initialize
M = 3000;   % set filter factors
L = M;      % set lenght L = M
s2d = 0.27; % variance
n = 1.2e6;  % set step
kmax = floor(n/L);    % set the max Block index
average_J = zeros(kmax-1);

% allocate
u = zeros(n,1);
W = zeros(2*M,1);
P = zeros(2*M,1);
u_ = zeros(2*M,1);
y_ = zeros(n,1);
e = zeros(n,1);
J = zeros(n,1);

% adding noise
v = sqrt(s2d)*randn(n,1); % Add gaussian noise 
v = v - mean(v);
u(1) = v(1);
for i=2:n
    u(i) = (-0.18*u(i-1)) + v(i); % noisy signal
end  
d = plant(u')';

for k=1:kmax-1
    u_ = u((k-1)*M+1:(k+1)*M);
    u_pair = fft(u_, 2*M);
    y_ = ifft(u_pair.*W);
    y = y_(M+1:2*M);
    d_pair = d(k*M+1:(k+1)*M);
    e(k*M+1:(k+1)*M ,1) = d_pair-y;
    average_J(k) = sum(e(k*M+1:(k+1)*M,1).^2)/M;

    U = fft([zeros(M,1)' e(k*M+1:(k+1)*M)']' ,2*M );

    g = 0.3; % set forgetting factor
    P = g*P+(1-g)*abs(u_pair).^2; % calc Pi
    D = 1./P;           

    f_ = ifft(D.*conj(u_pair).*U,2*M);
    f = f_(1:M);

    step = 0.4; % set step 
    W = W+step*fft([f;zeros(M,1)],2*M); % calc weight
end

w_t = ifft(W);
w_t = real(w_t(1:length(W)/2));

toc     % stop timer


% generate figure
figure(3)
semilogy(average_J);
xlabel('k (Block Iterations)')
ylabel('Ee^{2}(n)');
title('Learning curves - Implementation C');
