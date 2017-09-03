function [out, G] = apply_givens(in, sp_case)
  [G,y] = planerot(in(1,:).');
  if (sp_case == 1)
    a = exp(-1i*angle(in(1,1)))*ones(2,2);    
    a = a';
    a(2, 1) = -real(a(2, 1))+1i*imag(a(2, 1));
    a(1, 2) = -real(a(1, 2))+1i*imag(a(1, 2));
    G = G.*a;
    G(2,1) = conj(G(2,1));  
  elseif (sp_case == 2)
    G =  exp(-1i*angle(in(1,1)))*G;
  else
    G = G*exp(-1i*angle(in(1,1)));
  end
  
  out = G*in.';  
  out = out.';
end  