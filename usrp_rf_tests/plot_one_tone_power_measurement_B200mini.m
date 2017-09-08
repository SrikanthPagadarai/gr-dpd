clc;
clear; 
%close all;

center_freq = '500000000';
g = 400:10:890;
a = 5:5:70;

span = 1e6;
res_bw = 5.1e3;

pow = cell(length(a), length(g));
for jj = 1:length(a)
    for kk = 1:length(g)
        filename = strcat('./one_tone_power_measurements_B200mini/one_tone_power_B200mini_f', ...
            center_freq, '_g', int2str(g(kk)), '_a', int2str(a(jj)), '.bin');
        fileID = fopen(filename,'r');
        if fileID <0
            fprintf('File not found: a = %d\t\t g = %d\n', a(jj), g(kk));
        else
            A = fread(fileID,'*float')';
            A = reshape(A.', 1024, length(A)/1024);        
        end
        A = A(1:1001, :);

        pow_dbmHz = zeros(size(A,2), 1);
        pow{jj, kk} = zeros(size(A,2), 1);
        for ii = 1:size(A,2)
            pow_dbmHz(ii) = 10*log10(mean(10.^((A(:,ii)./10)))/res_bw);
            pow{jj, kk}(ii) = 10*log10(span*mean(10.^((A(:,ii)./10)))./res_bw);
        end
    end
end

mean_pow_lin = zeros(length(a), length(g));
mean_pow_dBm = zeros(length(a), length(g));
for jj = 1:length(a)
    y = a(jj)*ones(100,1)./100;
    for kk = 1:length(g)
        x = g(kk)*ones(100,1)./10;
        mean_pow_lin(jj, kk) = mean(10.^(0.1*pow{jj, kk}(2:end)));
        mean_pow_dBm(jj, kk) = 10*log10(mean_pow_lin(jj, kk));
    end
end


figure;
plot(g./10, mean_pow_dBm(1, :), 'k*-')
hold on;
grid on;
plot(g./10, mean_pow_dBm(2, :), 'k+-')
plot(g./10, mean_pow_dBm(3, :), 'k^-')
plot(g./10, mean_pow_dBm(4, :), 'k<-')
plot(g./10, mean_pow_dBm(5, :), 'k>-')
plot(g./10, mean_pow_dBm(6, :), 'kv-')
plot(g./10, mean_pow_dBm(7, :), 'ksq-')
plot(g./10, mean_pow_dBm(8, :), 'b*-')
plot(g./10, mean_pow_dBm(9, :), 'b+-')
plot(g./10, mean_pow_dBm(10, :), 'b^-')
plot(g./10, mean_pow_dBm(11, :), 'b<-')
plot(g./10, mean_pow_dBm(12, :), 'b>-')
plot(g./10, mean_pow_dBm(13, :), 'bv-')
plot(g./10, mean_pow_dBm(14, :), 'bsq-')
legend('Ampl. Scale = 0.05','Ampl. Scale = 0.1','Ampl. Scale = 0.15',...
    'Ampl. Scale = 0.2','Ampl. Scale = 0.25','Ampl. Scale = 0.3',...
    'Ampl. Scale = 0.35','Ampl. Scale = 0.4','Ampl. Scale = 0.45',...
    'Ampl. Scale = 0.5','Ampl. Scale = 0.55','Ampl. Scale = 0.6',...
    'Ampl. Scale = 0.65','Ampl. Scale = 0.7', 'Location', 'NorthWest');
xlim([40 89]);
xlabel('B200-Mini Gain (dB)');
ylabel('Transmit Power (dBm)');