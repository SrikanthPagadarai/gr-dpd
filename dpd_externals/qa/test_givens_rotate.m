#!/usr/bin/octave -qf

arg_list = argv ();
N = str2num(arg_list{1});
sp_case = str2num(arg_list{2});
suffix = arg_list{3};

in = randn(N, 2)+1i*randn(N, 2);
if (sp_case == 1)
    in(1,1) = 0;      
elseif (sp_case == 2)
    in(1,2) = 0;
end

[out, G] = apply_givens(in, sp_case);

in = real(in)+1i*imag(in);
out = real(out)+1i*imag(out);

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

theta_fn1 = strcat('theta_re_', suffix, '.txt');
fileID5 = fopen(theta_fn1,'w');
fprintf(fileID3,'%f\n',real(G(:)));
fclose(fileID3);

theta_fn2 = strcat('theta_im_', suffix, '.txt');
fileID6 = fopen(theta_fn2,'w');
fprintf(fileID4,'%f\n',imag(G(:)));
fclose(fileID4);
