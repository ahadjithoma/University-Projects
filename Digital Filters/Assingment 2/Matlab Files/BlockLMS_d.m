% implementation of Block-LMS algorithm (unconstrained-version)


clear all
close all
tic     % start timer

% initialize
M = 3000;               % set filter factors
L = M;                  % set lenght L = M+1
n = 1.4e6;              % set step
kmax = floor(n/L);      % set the max Block index
s2d = 0.27;             % variance

% allocate
u = zeros(n,1);
W = zeros(2*L,1);
y = zeros(n,1);
e = zeros(n,1);
average_J = zeros(kmax);

% noise
v = sqrt(s2d)*randn(n,1); % Add gaussian noise 
v = v - mean(v);
u(1) = v(1);
for i=2:n
    u(i) = (-0.18*u(i-1))+v(i);
end

d = plant(u.');
u = u.';
W = W.';

for k=2:kmax
    if k == 2
        p = var(u((k-1)*L+1:k*L))*autocorr(u((k-1)*L+1:k*L),L-1);
        mu = 0.4*(2/(L*eigs(toeplitz(p),1,'la')));
    end
    F_t = fft(u((k-2)*L+1:k*L));
    T_t = ifft(F_t.*W);
    y = T_t(L+1:2*L);
    e = d((k-1)*L+1:k*L)-y;
    E = fft([zeros(1,L) e]);
    W = W+mu*conj(F_t).*E;
    average_J(k) = sum(e.^2);
end
    
    
toc     % stop timer

figure(4)
semilogy(average_J)
xlabel('k (Block Iterations)')
ylabel('Ee^{2}(n)');
title('Learning curves - Implementation D');

