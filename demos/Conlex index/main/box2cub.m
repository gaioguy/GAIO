function box2cub(b, filename)

%% Converts a box collection, b, in GAIO to a list of cubes written in 'filename' for 'homcubes'.

fid = fopen(filename,'w');

d = (size(b,1)-2)/2;

fprintf(fid, 'dimension %d\n', d);

r = b(d+1:2*d,1);
for i=1:d
  b(i,:) = b(i,:)/r(i)/2;
end

q = round(b(1:d,:)-0.5)';
for i=1:size(q,1)
  fprintf(fid, '(');
  for j=1:d-1,
    fprintf(fid, '%d,', q(i,j));
  end
  fprintf(fid, '%d)\n', q(i,d));
end

fclose(fid);

