clear all
close all

mu = 3;
N = 1000; %number of time steps
n = 1:N;  %step

x = (cos(pi*n).*sin((pi/25)*n + pi/3))'; %signal
v = sqrt(0.19)*randn(N,1); v = v - mean(v); % noise
d = x + v;  %desired signal


%% Wiener filter input
u = zeros(N,1);
u(1) = v(1);

for i=2:N
    u(i) = -0.78 * u(i-1) + v(i);
end

%plot
figure(1)
plot([d x u])
legend({'d(n)', 'x(n)', 'u(n)'})


%% FIR filter
R = [0.4852 -0.3784; -0.3784 0.4852]; %autocorrelation Table

P = [0.19; 0];  %cross-correlation vector
wo = R\P;       

T = [u [0; u(1:N-1)]]; 

yo = T*wo; %filter

%plot
figure(2)
plot([d yo])
legend({'d(n)', 'y(n)'})


%% clean signal - error value
e = d - yo;

%plot
figure(3)
plot([e yo]);
legend({'e(n)', 'y(n)'});


%% Steepest descent appliance
w = [-1; -1]; 

%zero fill
wt = zeros([2,N]); 
wt(:,1) = w;
y = zeros(N, 1); 

s = [0; u];
for i=2:N
  w = w + mu*(P-R*w); % Adaptation steps
  wt(:,i) = w;
  y(i) = s(i:-1:i-1)' * w; % filter
end

%plot
figure(4)
plot([d y])
legend({'d(n)', 'y(n)'})


%% parameter error
we = (wt - wo*ones(1,N)).^2;
e = sqrt(sum(we));

%plot
figure(5)
semilogy(e);
xlabel('n: time step');
ylabel('value of parameter error');
title('Parameter Error');


%% contour curves and trajectories
L = 50;
ww = linspace(-3,3,L);

J = zeros([L,L]);
sigma2d = 0.1;

% error surface
for i=1:L
  for k=1:L
    wp = [ww(i); ww(k)];
    J(k,i) = sigma2d - 2*P'*wp + wp'*R*wp;
  end
end

min_J = min(J(:));
max_J = max(J(:));

levels = linspace(min_J,max_J,12);

%plot
figure(6)
contour(ww, ww, J, levels); axis square
hold on
plot(wt(1,:), wt(2,:), 'xr--');
hold off
xlabel('w(1)');
ylabel('w(2)');
title(['Error Surface: m = ', num2str(mu)]);


