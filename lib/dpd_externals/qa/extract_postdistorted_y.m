#!/usr/bin/octave -qf

arg_list = argv ();
K_a = str2num(arg_list{1});
L_a = str2num(arg_list{2});
K_b = str2num(arg_list{3});
M_b = str2num(arg_list{4});
L_b = str2num(arg_list{5});
suffix = arg_list{6};

len = K_a*(L_a+1)+K_b*M_b*(L_b+1);
in = randn(len, 1)+1i*randn(len, 1);
y_vec_i1 = zeros(1, K_a*L_a);
y_tmp1 = in(1:K_a*L_a+K_a);
y_tmp1 = reshape(y_tmp1, L_a+1, K_a);
y_tmp1(end, :) = [];    
y_vec_i1(1, 1:K_a*L_a) = y_tmp1(:);      

y_vec_i2_tmp = zeros(K_b*L_b, M_b);    
for mm = 1:M_b
    index = ( K_a*L_a+K_a ) + (mm-1)*(K_b*L_b+K_b) + (1:K_b*L_b+K_b); 
    y_tmp3 = in(index);
    y_tmp3 = reshape(y_tmp3, L_b+1, K_b);
    y_tmp3(end, :) = [];    
    y_vec_i2_tmp( 1:K_b*L_b, mm) = y_tmp3(:);       
    
end    
y_vec_i2 = y_vec_i2_tmp(:);    
out = [y_vec_i1   y_vec_i2.'];

in = single(real(in))+1i*single(imag(in));
out = single(real(out))+1i*single(imag(out));

in_fn1 = strcat('in_re_', suffix, '.txt');
fileID1 = fopen(in_fn1,'w');
fprintf(fileID1,'%f\n',real(in(:)));
fclose(fileID1);

in_fn2 = strcat('in_im_', suffix, '.txt');
fileID2 = fopen(in_fn2,'w');
fprintf(fileID2,'%f\n',imag(in(:)));
fclose(fileID2);

out_fn1 = strcat('out_re_', suffix, '.txt');
fileID3 = fopen(out_fn1,'w');
fprintf(fileID3,'%f\n',real(out(:)));
fclose(fileID3);

out_fn2 = strcat('out_im_', suffix, '.txt');
fileID4 = fopen(out_fn2,'w');
fprintf(fileID4,'%f\n',imag(out(:)));
fclose(fileID4);