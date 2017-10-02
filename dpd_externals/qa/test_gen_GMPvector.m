#!/usr/bin/octave -qf

arg_list = argv ();
K_a = str2num(arg_list{1});
L_a = str2num(arg_list{2});
K_b = str2num(arg_list{3});
M_b = str2num(arg_list{4});
L_b = str2num(arg_list{5});
item = str2num(arg_list{6});
N = str2num(arg_list{7});
suffix = arg_list{8};

y = 0.1*randn(N,1)+1i*0.1*randn(N,1);
history = max(L_a, M_b+L_b);
y = [zeros(history-1, 1); y];
in = y;

yy_cell = cell(1+M_b,1);
yy1 = zeros(1, K_a*(L_a+1)); 
yy2 = zeros(1, K_b*M_b*(L_b+1));    

%% create input to K_a channels
yy_cell{1,1} = zeros(K_a, L_a+1);
for kk=1:K_a        
    if item < L_a+1
        yy_cell{1,1}(kk, 1:L_a+1) = [y(item:-1:1); zeros(L_a+1-item, 1)].*...
            abs([y(item:-1:1); zeros(L_a+1-item, 1)]).^(kk-1);
        
    else
        yy_cell{1,1}(kk, 1:L_a+1) = y(item:-1:item-L_a).*abs(y(item:-1:item-L_a)).^(kk-1);
        
    end
    
    tmp_yy = yy_cell{1,1}.';
    yy1(1, 1:K_a*(L_a+1)) = tmp_yy(:).';
end


for mm=1:M_b
    %% create input to K_b channels
    yy_cell{mm+1,1} = zeros(K_b, L_b+1);
    
    for kk=1:K_b
        if (item-mm) > 0  
            if item < L_b+1
                yy_cell{mm+1,1}(kk, 1:item) = y(item:-1:1).*...
                        abs([y(item-mm:-1:1); zeros(mm, 1)]).^kk;
                    
            elseif ( (item == L_b+1) || (item > L_b+1)) && ( (item-mm) < L_b+1 )
                yy_cell{mm+1,1}(kk, :) = y(item:-1:item-L_b).*...
                    abs([y(item-mm:-1:1); zeros(L_b+1-(item-mm), 1)]).^kk;

            else
                yy_cell{mm+1,1}(kk, 1:L_b+1) = y(item:-1:item-L_b).*abs(y(item-mm:-1:item-L_b-mm)).^kk;

            end
        end
        
    end
    
    tmp_yy = yy_cell{mm+1,1}.';
    yy2(1, (mm-1)*K_b*(L_b+1) + (1:K_b*(L_b+1))) = tmp_yy(:).';
end

out = [yy1 yy2];
out = out(:);

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
