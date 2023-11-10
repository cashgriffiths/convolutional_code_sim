x = 2:7;
y_100 = [3.97e-1 1.37e-1 3.06e-2 5.25e-3 5.79e-4 3.16e-5];
y_1000 = [9.8e-1 8.33e-1 2.57e-1 4.43e-02 4.89e-03 3.49e-04];
y_5000 = [1 1 8.40e-01 2.61e-01 2.55e-02 1.55e-03];

pb1 = [1.79e-02 4.43e-03 8.67e-04 1.41e-04 1.29e-05 6.80e-07];
pb2 = [2.37e-02 1.18e-02 3.18e-03 5.19e-04 5.58e-05 3.91e-06];
pb3 = [2.53e-02 1.39e-02 9.07e-03 2.71e-03 2.61e-04 1.58e-05];
A = zeros(6, 3);
A(:,1) = pb1;
A(:,2) = pb2;
A(:,3) = pb3;
pb = mean(A, 2);

semilogy(x, y_100,'LineWidth',2), grid, hold on
% semilogy(x, y_ht,'LineWidth',2)
semilogy(x, y_1000,'LineWidth',2)
% semilogy(x, y_st,'LineWidth',2)
semilogy(x, y_5000,'LineWidth',2)
semilogy(x, pb,'LineWidth',2)
xlim([1 9])
xlabel('Eb/N0 (dB)')
ylabel('Codeword error probability')
legend('k=100', 'k=1000', 'k=5000', 'bit error, mu=2')
axis([1 9 1e-5 1])
