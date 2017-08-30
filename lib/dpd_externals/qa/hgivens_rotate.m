#!/usr/bin/octave -qf

arg_list = argv ();
N = str2num(arg_list{1});
sp_case = str2num(arg_list{2});
suffix = arg_list{3};

in = randn(N, 2)+1i*randn(N, 2);
if (sp_case == 1)
    in(1,1) = 0;      
end

if abs(in(1, 1)) == 0
  out = fliplr(in);
elseif abs(in(1, 2)) == 0
  out = in;
else 
  x = in(1, 1);
  y = in(1, 2);    
  if abs(x) < abs(y)
      flip = 1;
      b = x;
      a = y;
  else 
      flip = 0;
      b = y;
      a = x;
  end
  conj_a = conj(a);
  conj_b = conj(b);
  abs_a = abs(a);
  abs_b = abs(b);

  out = zeros(size(in));
  for ii = 1:size(in, 1)
      if (flip)
          x = in(ii, 2);
          y = in(ii, 1);    
      else
          x = in(ii, 1);
          y = in(ii, 2);    
      end
      c = conj_b*y/(conj_a*x);
      
      if (abs(c) < 0.5)
          zeta = 1-c;
      else
          d1 = (conj_a-conj_b)/conj_a;
          d2 = (x-y)/x;
          zeta = d1+d2-d1*d2;
      end
      angle_x = angle(x);
      out(ii, 1) = exp(-1i*angle_x)*(abs_a * x * zeta)/sqrt((abs_a - abs_b) * (abs_a + abs_b));
      out(ii, 2) = out(ii, 1) - ( conj_a*x - conj_b*y )/sqrt((abs_a - abs_b) * (abs_a + abs_b));
  end

  if (flip)
      out = fliplr(out);
  end
end

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