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

out = apply_hgivens(in);

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