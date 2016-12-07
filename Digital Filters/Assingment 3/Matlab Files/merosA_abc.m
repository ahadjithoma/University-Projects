clear all
close all

N = 5000; 
n = 1:N;  
factors = 100;

A = 2.3;    
fo = 1 / 4;
D = 10;
phi = pi / 2;
s2v = 0.34;

x = A * (sin(2 * pi * fo * n + phi) + cos(4 * pi * fo * n + phi) + cos(7 * pi * fo * n + phi / 3));
v = sqrt(s2v) * randn(1, N);
v = v - mean(v);
d = x + v; 
u(n + D) = d(n);

%% Question B - Wiener filter 

r = var(d) * autocorr(d, factors - 1);
R = toeplitz(r);
P(1) = s2v; 
P(2:factors) = 0;
wo = R \ P';
y = filter(wo, 1, u(D + 1:size(u, 2)));
semilogy((y - v).^2)
hold on

%% Question G - levinson-durbin

r = var(d) * autocorr(d, factors - 1);

%LD - iterative
[a_i, G, L, Dp] = LevinsonDurbin_iterative(factors - 1, r'); 

%LD - recursive
[a, fpp, p] = LevinsonDurbin(factors - 1, r');

% LD Matlab 
e(1:factors - 1) = 0;
for j = 0:factors - 1
    [al, e(j + 1)] = levinson(r', j); 
end

% display error
fprintf('Norm of difference with MATLAB levinson (iterative) %e\n', ...
    norm(a_i - al'))
fprintf('Norm of difference with MATLAB levinson (recursive)   %e\n', ...
    norm(a - al')) 
fprintf('Norm of difference with gama coefficients and Wiener %e\n', ...
   norm(((L')*al')' - wo')) 
fprintf('Norm of difference with Matlab backward errors %e\n', ...
    norm(e - Dp'))

% filter output - plot
y = filter(a, 1, u(D + 1:size(u, 2)));
hold on
semilogy((y - v).^2,'r')
title('Square Error of Wiener and Levinson-Durbin')
ylabel('Square Error ( E[e^2(n)] )');
xlabel('Steps (n)')
legend('Wiener','Levinson Durbin')



