#!/usr/bin/octave -qf

arg_list = argv ();
N = str2num(arg_list{1});
suffix = arg_list{2};

in = randn(N,1)+1i*randn(N,1);
w = [0.0137723135750602, 0.0473339698075019, 0.119480615091568,	0.221503433838812,	0.301593494919519,	0.301593494919519,	0.221503433838812,	0.119480615091568,	0.0473339698075019,	0.0137723135750602];
out = filter(w, 1, in);

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
