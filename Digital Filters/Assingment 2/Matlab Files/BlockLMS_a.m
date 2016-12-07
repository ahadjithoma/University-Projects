% implementation of Block-LMS algorithm with 2 nested loops
% based on adaptLMS.m by Sismanis Nikos, 28 May 2013


clear all
close all

tic     % start timer

% initialize
M = 3000;   % set filter factors
L = M;      % set lenght L = M
s2d = 0.27; % variance
n = 1.2e6;  % set step
mu = 0.0005;
kmax = floor(n/L);    % set the max Block index
    


% alocate
average_J = zeros(kmax-1);  % Average errors for Graph
u = zeros(n,1);
w = zeros(M,1);
y = zeros(n,1);
e = zeros(n,1);
J = zeros(n,1);

% adding noise
v = sqrt(s2d)*randn(n,1); % Add gaussian noise 
v = v-mean(v);
u(1) = v(1);
for i=2:n
    u(i) = (-0.18*u(i-1))+v(i); % noisy signal
end

d = plant(u')';


% (nested) loop1
for k=1:kmax-1
    f = zeros(M,1);
    u_pair = zeros(M,1);

    % (nested) loop2
    for i=1:L-1

        % Read/Generate a new data pair (u,d)
        u_pair = u(k*L+i:-1:k*L+i-M+1);
        d_pair = d(k*L+i);     

        y = w'*u_pair;       % filter output
        e = d_pair-y;        % output error
        f = f+(mu*e*u_pair); % accumulate
        J = e*e;
        average_J(k) = average_J(k)+J;
    end
    average_J(k) = average_J(k)/L;
    w = w + f;
end     

toc     % stop timer

% generate figure
figure(1)
semilogy(average_J);
xlabel('k (Block Iterations)')
ylabel('Ee^{2}(n)');
legend({sprintf('mu=%f',mu)});
title('Learning curves - Implementation A');


