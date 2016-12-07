clear all
load('music.mat')

D = 100; 
factors = 200;

%% filter
 
r = var(s) * autocorr(s, factors-1);
a = LevinsonDurbin(factors - 1, r); 
n = 1:size(s, 1);
s = s';
u(n + D) = s(n);

%% filter output -> play the music

y = filter(a, 1 , u);
sound(100 * y, fs)
