function dispr(s,n)

persistent l;
if n == 0, l = 0; end
if l > 0
   for j=1:l+1, fprintf('\b'); end;
end
l = fprintf(s);
