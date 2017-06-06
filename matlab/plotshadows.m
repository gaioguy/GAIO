function plotshadows(Q, B)

% b is the outer big box, s are the boxes of the covering (for the shadows)

d = 3;
[az, el] = view

% back
xQ = [Q(:,1)-Q(:,1+d) Q(:,1)+Q(:,1+d) Q(:,1)+Q(:,1+d) Q(:,1)-Q(:,1+d)];
yQ = [Q(:,2)-Q(:,2+d) Q(:,2)-Q(:,2+d) Q(:,2)-Q(:,2+d) Q(:,2)-Q(:,2+d)];
zQ = [Q(:,3)-Q(:,3+d) Q(:,3)-Q(:,3+d) Q(:,3)+Q(:,3+d) Q(:,3)+Q(:,3+d)];
xB = [B(:,1)-B(:,1+d) B(:,1)+B(:,1+d) B(:,1)+B(:,1+d) B(:,1)-B(:,1+d)];
yB = kron([Q(:,2)-Q(:,2+d) Q(:,2)-Q(:,2+d) Q(:,2)-Q(:,2+d) Q(:,2)-Q(:,2+d)]*0.95, ones(size(xB,1),1));
zB = [B(:,3)-B(:,3+d) B(:,3)-B(:,3+d) B(:,3)+B(:,3+d) B(:,3)+B(:,3+d)];
if (az > 90) | (az < -90), 
    p = patch(xQ',yQ',zQ',[0.8 0.8 0.8],'edgealpha',0); 
    patch(xB',yB',zB','k','edgealpha',0,'facealpha',0.5); 
end
% front
xQ = [Q(:,1)-Q(:,1+d) Q(:,1)+Q(:,1+d) Q(:,1)+Q(:,1+d) Q(:,1)-Q(:,1+d)];
yQ = [Q(:,2)+Q(:,2+d) Q(:,2)+Q(:,2+d) Q(:,2)+Q(:,2+d) Q(:,2)+Q(:,2+d)];
zQ = [Q(:,3)-Q(:,3+d) Q(:,3)-Q(:,3+d) Q(:,3)+Q(:,3+d) Q(:,3)+Q(:,3+d)];
xB = [B(:,1)-B(:,1+d) B(:,1)+B(:,1+d) B(:,1)+B(:,1+d) B(:,1)-B(:,1+d)];
yB = kron([Q(:,2)+Q(:,2+d) Q(:,2)+Q(:,2+d) Q(:,2)+Q(:,2+d) Q(:,2)+Q(:,2+d)]*0.95, ones(size(xB,1),1));
zB = [B(:,3)-B(:,3+d) B(:,3)-B(:,3+d) B(:,3)+B(:,3+d) B(:,3)+B(:,3+d)];
if az > -90 & az < 90, 
    patch(xQ',yQ',zQ',[0.8 0.8 0.8],'edgealpha',0); 
    patch(xB',yB',zB','k','edgealpha',0,'facealpha',0.5); 
end
% right
xQ = [Q(:,1)+Q(:,1+d) Q(:,1)+Q(:,1+d) Q(:,1)+Q(:,1+d) Q(:,1)+Q(:,1+d)];
yQ = [Q(:,2)-Q(:,2+d) Q(:,2)+Q(:,2+d) Q(:,2)+Q(:,2+d) Q(:,2)-Q(:,2+d)];
zQ = [Q(:,3)-Q(:,3+d) Q(:,3)-Q(:,3+d) Q(:,3)+Q(:,3+d) Q(:,3)+Q(:,3+d)];
yB = [B(:,2)-B(:,2+d) B(:,2)+B(:,2+d) B(:,2)+B(:,2+d) B(:,2)-B(:,2+d)];
zB = [B(:,3)-B(:,3+d) B(:,3)-B(:,3+d) B(:,3)+B(:,3+d) B(:,3)+B(:,3+d)];
xB = kron([Q(:,1)+Q(:,1+d) Q(:,1)+Q(:,1+d) Q(:,1)+Q(:,1+d) Q(:,1)+Q(:,1+d)]*0.95,  ones(size(xB,1),1));
if az < 0, 
    patch(xQ',yQ',zQ',[0.8 0.8 0.8],'edgealpha',0); 
    patch(xB',yB',zB','k','edgealpha',0,'facealpha',0.5); 
end
% left
xQ = [Q(:,1)-Q(:,1+d) Q(:,1)-Q(:,1+d) Q(:,1)-Q(:,1+d) Q(:,1)-Q(:,1+d)];
yQ = [Q(:,2)-Q(:,2+d) Q(:,2)+Q(:,2+d) Q(:,2)+Q(:,2+d) Q(:,2)-Q(:,2+d)];
zQ = [Q(:,3)-Q(:,3+d) Q(:,3)-Q(:,3+d) Q(:,3)+Q(:,3+d) Q(:,3)+Q(:,3+d)];
yB = [B(:,2)-B(:,2+d) B(:,2)+B(:,2+d) B(:,2)+B(:,2+d) B(:,2)-B(:,2+d)];
zB = [B(:,3)-B(:,3+d) B(:,3)-B(:,3+d) B(:,3)+B(:,3+d) B(:,3)+B(:,3+d)];
xB = kron([Q(:,1)-Q(:,1+d) Q(:,1)-Q(:,1+d) Q(:,1)-Q(:,1+d) Q(:,1)-Q(:,1+d)]*0.95,  ones(size(xB,1),1));
if az > 0, 
    patch(xQ',yQ',zQ',[0.8 0.8 0.8],'edgealpha',0); 
    patch(xB',yB',zB','k','edgealpha',0,'facealpha',0.5); 
end
% top
xQ = [Q(:,1)-Q(:,1+d) Q(:,1)+Q(:,1+d) Q(:,1)+Q(:,1+d) Q(:,1)-Q(:,1+d)];
yQ = [Q(:,2)-Q(:,2+d) Q(:,2)-Q(:,2+d) Q(:,2)+Q(:,2+d) Q(:,2)+Q(:,2+d)];
zQ = [Q(:,3)+Q(:,3+d) Q(:,3)+Q(:,3+d) Q(:,3)+Q(:,3+d) Q(:,3)+Q(:,3+d)];
xB = [B(:,1)-B(:,1+d) B(:,1)+B(:,1+d) B(:,1)+B(:,1+d) B(:,1)-B(:,1+d)];
yB = [B(:,2)-B(:,2+d) B(:,2)-B(:,2+d) B(:,2)+B(:,2+d) B(:,2)+B(:,2+d)];
zB = kron([Q(:,3)+Q(:,3+d) Q(:,3)+Q(:,3+d) Q(:,3)+Q(:,3+d) Q(:,3)+Q(:,3+d)]*0.95, ones(size(xB,1),1));
if el < 0, 
    patch(xQ',yQ',zQ',[0.8 0.8 0.8],'edgealpha',0); 
    patch(xB',yB',zB','k','edgealpha',0,'facealpha',0.5); 
end
% bottom
xQ = [Q(:,1)-Q(:,1+d) Q(:,1)+Q(:,1+d) Q(:,1)+Q(:,1+d) Q(:,1)-Q(:,1+d)];
yQ = [Q(:,2)-Q(:,2+d) Q(:,2)-Q(:,2+d) Q(:,2)+Q(:,2+d) Q(:,2)+Q(:,2+d)];
zQ = [Q(:,3)-Q(:,3+d) Q(:,3)-Q(:,3+d) Q(:,3)-Q(:,3+d) Q(:,3)-Q(:,3+d)];
xB = [B(:,1)-B(:,1+d) B(:,1)+B(:,1+d) B(:,1)+B(:,1+d) B(:,1)-B(:,1+d)];
yB = [B(:,2)-B(:,2+d) B(:,2)-B(:,2+d) B(:,2)+B(:,2+d) B(:,2)+B(:,2+d)];
zB = kron([Q(:,3)-Q(:,3+d) Q(:,3)-Q(:,3+d) Q(:,3)-Q(:,3+d) Q(:,3)-Q(:,3+d)]*0.95, ones(size(xB,1),1));
if el > 0, 
    patch(xQ',yQ',zQ',[0.8 0.8 0.8],'edgealpha',0); 
    patch(xB',yB',zB','k','edgealpha',0,'facealpha',0.5); 
end



