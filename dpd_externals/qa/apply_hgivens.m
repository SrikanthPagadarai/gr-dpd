function out = apply_hgivens(in)
  z = in(1, :);
  if abs(z(1)) == 0
    angle_a = angle(z(2));
    out = exp(-1i*angle_a)*in;
  elseif abs(z(2)) == 0
    angle_a = angle(z(1));
    out = exp(-1i*angle_a)*in;
  else     
    if ( abs(z(1)) > abs(z(2)) )            
        rho = z(2)/z(1);
        theta = exp( -1i*angle(z(1)) )*( 1/sqrt(1-abs(rho)^2) )*[1 -rho; -rho' 1];         
        out = in*theta;
    elseif ( abs(z(1)) < abs(z(2)) )            
        rho = z(1)'/z(2)';
        theta = exp( -1i*angle(z(2)) )*( 1/sqrt(1-abs(rho)^2) )*[1 -rho; -rho' 1];                    
        out = in*theta;
    end
  end
end

%{
function out = apply_hgivens(in)
  if abs(in(1, 1)) == 0
    angle_a = angle(in(1,2));
    out = exp(-1i*angle_a)*fliplr(in);
  elseif abs(in(1, 2)) == 0
    angle_a = angle(in(1,1));
    out = exp(-1i*angle_a)*in;
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
    angle_a = angle(a);
    
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
        
        out(ii, 1) = (abs_a * x * zeta)/sqrt((abs_a - abs_b) * (abs_a + abs_b));
        out(ii, 2) = out(ii, 1) - ( conj_a*x - conj_b*y )/sqrt((abs_a - abs_b) * (abs_a + abs_b));
        out(ii, 1) = exp(-1i*angle_a)*out(ii, 1);
        out(ii, 2) = exp(-1i*angle_a)*out(ii, 2);
    end

    if (flip)
        out = fliplr(out);
    end
  end
end
%}