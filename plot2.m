
y_100 = [7.35e-01 3.97e-1 1.37e-1 3.06e-2 5.25e-3 5.79e-4 3.16e-5];
y_1000 = [1 9.8e-1 8.33e-1 2.57e-1 4.43e-02 4.89e-03 3.49e-04];
y_5000 = [1 1 1 8.40e-01 2.61e-01 2.55e-02 1.55e-03];
n = 11;
x = 1:n;
p = zeros(1,n);
for snr=x
    p(snr) = qfunc(sqrt(2*10^(snr/10)));
end

u_100 = 1-(1-p).^100;
u_1000 = 1-(1-p).^1000;
u_5000 = 1-(1-p).^5000;

semilogy(1:7, y_100,'LineWidth',2), grid, hold on
semilogy(x, u_100,'LineWidth',2)

xlim([1 n+1])

legend('k=100', 'k=100, uncoded')