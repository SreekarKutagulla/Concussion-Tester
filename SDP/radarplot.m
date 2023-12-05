function radarplot(X,Lable,color)
 % creates a radar (spider) plot for 1 data series.
 %Example:
 % radarplot([1 ; 2; 0 ; 3; 2 ],{ 'a' ; 'b' ; 'c' ; 'd' ; 'e' } ,'r')
%INPUT: 
%X: numeric vector Nx1
%Lable: cell of string vector Nx1
%color: char, one of the matlab colors
 
 if size(X,2)~=1 || size(Lable,2)~=1 || size(X,1)~=size(Lable,1)
     error('X and Lable must have equale length Nx1 ')
 end
 if ~ischar(color) || sum(strcmpi({'y' 'm' 'c' 'r' 'g' 'b' 'w' 'k'},color))==0
     error('color must be a char of the following:  y, m, c, r, g, b, w, k')
 end
 % create 2 vectors angel (Angle) and raius (X) and convert from Polaric
 % to Cartesian
    Angle1=repmat(2*pi/size(X,1),size(X,1),1);
    Angle=cumsum([ 0 ; 0 ; Angle1]);
    X=[0 ;X ; X(1,1)];
    [CordX,CordY] = pol2cart(Angle,X);
    % create the cordinate to plot the grid (CordXG CordYG) and to plot
    % the lables (CordYL)
   CordXG=zeros(1,(size(X,1)-2)*2);
   CordYG=zeros(1,(size(X,1)-2)*2);
   CordXL=zeros(1,(size(X,1)-2));
   CordYL=zeros(1,(size(X,1)-2));
    for i=1:size(X,1)-2
        
        [CordXG(i*2-1) CordYG(i*2-1)]=pol2cart(Angle1(1)*(i-1),0);
        [CordXG(i*2) CordYG(i*2)]=pol2cart(Angle1(1)*(i-1),max(X));
        [CordXL(i) CordYL(i)]=pol2cart(Angle1(1)*(i-1)*1,max(X)*1.1);
    end
    % plot a Filled 2-D polygons and a normal line plot on the top of it
    fill(CordX,CordY,color)
    hold on
    plot(CordX,CordY,color,'LineWidth',3)
    hold on
    % plot grid
    plot(CordXG,CordYG,'k')
    %set axis
    axis([-max(X)*1.1 max(X)*1.1 -max(X)*1.1 max(X)*1.1])
    axis off
    % plot grid circle and value lables (notice you need the circle.m
    % function from the matlab flie exchence)
    Angle2=Angle(2:end);
    for i=1:5
        Rad=repmat((max(X)/5)*i,size(Angle2));
        [CordXS CordYS]=pol2cart(Angle2,Rad);
        plot(CordXS, CordYS,':k')
        text((max(X)/5/3),(max(X)/5)*i,num2str((max(X)/5)*i,2),'FontSize',6)
    end
    %plot text lables on the graph
    for i=1:size(CordXL,2)
        text(CordXL(i), CordYL(i),cell2mat(Lable(i)), 'FontSize',9,'HorizontalAlignment','center','Rotation',0)
    end
    