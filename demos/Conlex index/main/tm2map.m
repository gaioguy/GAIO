function tm2map(tree, depth, X, Y, P, filename)

%  tm2map writes map given by the transition matrix P with domain X and range Y 
%  to file to be read by 'homcubes'

fid = fopen(filename,'w');
b = tree.boxes(depth);

% space dimension
d = (size(b,1)-2)/2;

% box radius
r = b(d+1:2*d,1)';

% rescaling to integer lattice
for i=1:d
  b(i,:) = b(i,:)/r(i)/2;
  b(i+d,:) = b(i+d,:)/r(i)/2;
end

% for domain boxes in X, image is restricted to Y
for i=1:size(b(:,X),2),
  % for domain box i: center, lower left corner
    c = b(1:d,X(i))';
    ll = round(c-0.5);
    
    % print domain box	
    fprintf(fid, '(');
  for j=1:d-1, fprintf(fid, '%d,', ll(j)); end
    fprintf(fid, '%d) ' , ll(d));
  
  % compute image (restricted to Y) 
    Im = find(P(:,X(i)));
    Im = intersect(Im,Y);
  fprintf(fid, ' -> {');
  if (~isempty(Im))  
  % print image of box i
    for k=1:length(Im)-1,
         % kth box in image 
	 c = b(1:d,Im(k))';
    	 ll = round(c-0.5);
	 
	 % print kth box in image
	 fprintf(fid, '(');
	 for j=1:d-1, fprintf(fid, '%d,', ll(j)); end
   	 fprintf(fid, '%d) ', ll(d));	
     end
    % last box
	 % last box in image    	
	 c = b(1:d,Im(length(Im)))';
    	 ll = round(c-0.5);
   	 	 
	 % print kth box in image
	 fprintf(fid, '(');
	 for j=1:d-1, fprintf(fid, '%d,', ll(j)); end
   	 fprintf(fid, '%d)', ll(d));
   else
        fprintf('WARNING: box no %d has empty image. \n', X(i));
   end
   fprintf(fid, '}\n');	     	 
end



fclose(fid);
