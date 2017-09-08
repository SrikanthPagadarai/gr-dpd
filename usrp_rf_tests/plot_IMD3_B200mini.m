clc;
clear; 
%close all;

center_freq = '500000000';
g = 400:10:890;
a = 5:5:35;

span = 1e6;
res_bw = 5.1e3;

delta = span/1000;
f = -0.5*span:delta:0.5*span-delta;
ind1 = find(f == 2.5e4);
ind2 = find(f == 1e5);
ind3 = find(f == 5e4);
ind4 = find(f == 7.5e4);

mean_imd3_lvl_dBc = zeros(length(a), length(g));
for jj = 1:length(a)
    for kk = 1:length(g)
        filename = strcat('./two_tone_power_measurements_B200mini/two_tone_power_B200mini_f', ...
            center_freq, '_g', int2str(g(kk)), '_a', int2str(a(jj)), '.bin');
        fileID = fopen(filename,'r');
        if fileID <0
            fprintf('File not found: a = %d\t\t g = %d\n', a(jj), g(kk));
        else
            A = fread(fileID,'*float')';
            A = reshape(A.', 1024, length(A)/1024);        
        end
        A = A(1:1001, :);

        imd3_lvl_dBc = zeros(1, size(A,2)-1);
        for ii = 2:size(A,2)
            imd3_lvl_dBc(ii-1) = max(A(ind1, ii), A(ind2, ii)) - ...
                min(A(ind3, ii), A(ind4, ii));    
        end
        mean_imd3_lvl_lin = mean(10.^(0.1*imd3_lvl_dBc));
        mean_imd3_lvl_dBc(jj, kk) = 10*log10(mean_imd3_lvl_lin);
    end
end

figure;
plot(g./10, mean_imd3_lvl_dBc(1, :), 'k*-')
hold on;
grid on;
plot(g./10, mean_imd3_lvl_dBc(2, :), 'k+-')
plot(g./10, mean_imd3_lvl_dBc(3, :), 'k^-')
plot(g./10, mean_imd3_lvl_dBc(4, :), 'k<-')
plot(g./10, mean_imd3_lvl_dBc(5, :), 'k>-')
plot(g./10, mean_imd3_lvl_dBc(6, :), 'kv-')
plot(g./10, mean_imd3_lvl_dBc(7, :), 'ksq-')
legend('Ampl. Scale = 0.05','Ampl. Scale = 0.1','Ampl. Scale = 0.15',...
    'Ampl. Scale = 0.2','Ampl. Scale = 0.25','Ampl. Scale = 0.3',...
    'Ampl. Scale = 0.35');
xlim([40 89]);
xlabel('B200-Mini Gain (dB)');
ylabel('Mean IMD3 Level (dBc)');