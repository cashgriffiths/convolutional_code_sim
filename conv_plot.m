x = 1:7;
y_100 = [7.35e-01 3.97e-1 1.37e-1 3.06e-2 5.25e-3 5.79e-4 3.16e-5];
y_1000 = [1 9.8e-1 8.33e-1 2.57e-1 4.43e-02 4.89e-03 3.49e-04];
y_5000 = [1 1 1 8.40e-01 2.61e-01 2.55e-02 1.55e-03];

pb_2 = [4.25e-02 1.79e-02 4.43e-03 8.67e-04 1.41e-04 1.29e-05 6.80e-07];

pb_6 = [3.54e-02 4.3e-03 4.46e-04 2.17e-05 7.56e-07];

semilogy(x, y_100,'LineWidth',2), grid, hold on
% semilogy(x, y_ht,'LineWidth',2)
semilogy(x, y_1000,'LineWidth',2)
% semilogy(x, y_st,'LineWidth',2)
semilogy(x, y_5000,'LineWidth',2)
semilogy(x, pb_2,'LineWidth',2)
semilogy(1:5, pb_6,'LineWidth',2)
xlim([1 9])
xlabel('Eb/N0 (dB)')
ylabel('Codeword error probability')
legend('k=100', 'k=1000', 'k=5000', 'Pb, mu=2', 'Pb, mu=6')
axis([0 8 1e-6 1])
