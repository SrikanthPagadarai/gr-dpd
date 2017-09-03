function B = rotations(A)
  A_bar = A;
  for ii = 2:size(A,2)
      z = [A_bar(:, 1)    A_bar(:, ii)];
      
      switch mod(ii, 2)
          case 0
              [out, G] = apply_givens(z, 0);
          case 1
              out = apply_hgivens(z);
      end        
      
      B = [ out(:, 1)    A_bar(:, 2:ii-1) ...
            out(:, 2)    A_bar(:, ii+1:end) ];    
      A_bar = B;
  end
end  