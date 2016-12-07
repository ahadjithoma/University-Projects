% "Proof" by MATLAB
% A simple technique to develop and verify the steps of a proof 
% using random data input
%
% N P P
% Cornell U 
% Sept 1992
%

clear

n = 8; % any even
q = 3;

% input 
x = randn(n,1) + 1i*randn(n,1);
% correct answer
ys = fft(x);


% root of unity 
w = @(n,e) exp(-2*pi*1i.*e/n); % function = @(args) <function body>

% set k
k = (0:n-1)'; % k = 0,1,...,7

% DFT proof steps
y = zeros(n,1); 

for j = 0:n-1
  y(j +1) = sum(w(n,j*k) .* x(k +1));
end


fprintf('DFT : %e\n', norm(y - ys))

% split output top bottom
y = zeros(n,1);

for j = 0:n/2-1
  y(j +1) = sum(w(n,j*k) .* x(k +1));
end
for j = n/2:n-1
  y(j +1) = sum(w(n,j*k) .* x(k +1));
end

fprintf('split output top bottom : %e\n', norm(y - ys))

% split input even odd
y = zeros(n,1);

k = (0:n/2-1)'; % k = 0,1,2,3
for j = 0:n/2-1
  y(j +1) = sum(w(n,j*2*k) .* x(2*k +1)) + sum(w(n,j*(2*k+1)) .* x(2*k+1 +1));
end
for j = n/2:n-1
  y(j +1) = sum(w(n,j*2*k) .* x(2*k +1)) + sum(w(n,j*(2*k+1)) .* x(2*k+1 +1));
end

fprintf('split input even odd : %e\n', norm(y - ys))



%%

k = (0:n/2-1); % get k

% allocate x1, x2 and Wn
x1 = zeros(n/2-1,1);
x2 = zeros(n/2-1,1);
Wn = zeros(length(k),length(k));

% calc x1 & x2
for j = 0:n/2-1
  x1(j+1) = sum(w(n/2,j*k) * x(2*k+1));
  x2(j+1) = sum(w(n/2,j*k) * x(2*k+2));
end

% calc Wn*X2
WnX2 = w(n,k') .* x2; 

% calc answer
y = [x1 + WnX2; x1 - WnX2];

% print
fprintf('using DFT definition: %e\n', norm(y - ys))

%%


fe = fft(x((0:2:n-1) +1));
fo = fft(x((1:2:n-1) +1));


wfo = w(n,(0:n/2-1)') .* fo; 
y = [fe + wfo; fe - wfo];

fprintf('done : %e\n', norm(y - ys))


% Epalitheusi

%display(fe)
%display(x1)

%display(fo)
%display(x2)

%display(wfo)
%display(WnX2)



