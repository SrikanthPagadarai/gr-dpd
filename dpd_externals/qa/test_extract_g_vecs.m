#!/usr/bin/octave -qf

arg_list = argv ();
K_a = str2num(arg_list{1});
L_a = str2num(arg_list{2});
K_b = str2num(arg_list{3});
M_b = str2num(arg_list{4});
L_b = str2num(arg_list{5});
suffix = arg_list{6};

M = K_a*L_a+K_b*M_b*L_b;
M_bar = K_a+K_b*M_b;
in = randn(M+M_bar, 1)+1i*randn(M+M_bar, 1);
g_vec_i1 = zeros(K_a*L_a, 1);
g_tmp1 = in(1:K_a*L_a+K_a);
g_tmp1 = reshape(g_tmp1, L_a+1, K_a);
g_tmp1(end, :) = [];    
g_vec_i1( 1:K_a*L_a, 1) = g_tmp1(:);
g_tmp2 = [zeros(1, K_a) ; g_tmp1];
g_vec_iMinus1_a = g_tmp2(:); 

g_vec_i2_tmp = zeros(K_b*L_b, M_b);
g_vec_iMinus1_tmp = zeros(K_b*L_b+K_b, M_b);
for mm = 1:M_b
    index = ( K_a*L_a+K_a ) + (mm-1)*(K_b*L_b+K_b) + (1:K_b*L_b+K_b); 
    g_tmp3 = in(index);
    g_tmp3 = reshape(g_tmp3, L_b+1, K_b);
    g_tmp3(end, :) = [];    
    g_vec_i2_tmp( 1:K_b*L_b, mm) = g_tmp3(:);
    
    
    g_tmp4 = [zeros(1, K_b) ; g_tmp3];
    g_vec_iMinus1_tmp(:, mm) = g_tmp4(:);
end    
g_vec_i2 = g_vec_i2_tmp(:);
g_vec_iMinus1_b = g_vec_iMinus1_tmp(:);

out1 = [g_vec_iMinus1_a; g_vec_iMinus1_b];
out2 = [g_vec_i1; g_vec_i2];

in = single(real(in))+1i*single(imag(in));
out1 = single(real(out1))+1i*single(imag(out1));
out2 = single(real(out2))+1i*single(imag(out2));

in_fn1 = strcat('in_re_', suffix, '.txt');
fileID1 = fopen(in_fn1,'w');
fprintf(fileID1,'%f\n',real(in(:)));
fclose(fileID1);

in_fn2 = strcat('in_im_', suffix, '.txt');
fileID2 = fopen(in_fn2,'w');
fprintf(fileID2,'%f\n',imag(in(:)));
fclose(fileID2);

out1_fn1 = strcat('out1_re_', suffix, '.txt');
fileID3 = fopen(out1_fn1,'w');
fprintf(fileID3,'%f\n',real(out1(:)));
fclose(fileID3);

out1_fn2 = strcat('out1_im_', suffix, '.txt');
fileID4 = fopen(out1_fn2,'w');
fprintf(fileID4,'%f\n',imag(out1(:)));
fclose(fileID4);

out2_fn1 = strcat('out2_re_', suffix, '.txt');
fileID5 = fopen(out2_fn1,'w');
fprintf(fileID5,'%f\n',real(out2(:)));
fclose(fileID5);

out2_fn2 = strcat('out2_im_', suffix, '.txt');
fileID6 = fopen(out2_fn2,'w');
fprintf(fileID6,'%f\n',imag(out2(:)));
fclose(fileID6);