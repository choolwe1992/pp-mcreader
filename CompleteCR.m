function CompleteCR(str)

im = imread(str);

% pre-process the image

im = rgb2gray(im);
im = imresize(im,[250 600]);

im = medfilt2(im, [5 5]);
imbw = im2bw(im,graythresh(im));
figure,imshow(imbw);

imc = imcomplement(imbw);
figure,imshow(imc);

% detect rotation and de-skew
bwlab = bwlabel(imc);
% Objects = regionprops(bwlab,'Area','Orientation');
% [marea ind] = max([Objects.Area]);
% 
% bwrot = imrotate(bwlab,-Objects(ind).Orientation);
% figure,imshow(bwrot);

%------------------- CASE THIS IS THE FRONT ---------------------
% perform bw-label and region-props to get the bounding boxes closest to
% center
Objects = regionprops(bwlab,'BoundingBox','Area','Orientation','Centroid');
size(Objects);

[m,im] = max([Objects.Area]);

% If any this is the front side of the note:
dist_Thresh = 40;
count = 0;
idx = [];
for i=1:size(Objects,1)
    if(Objects(i).Area > 1500 & Objects(i).Area < 3500)
        idx = [idx i];
    end
    
end

flag100 = 0;

for i=1:size(idx,2)
    Objects(idx(i)).Centroid(1)
    Objects(idx(i)).Centroid(2)
    int16(Objects(im).BoundingBox(4)/2)
    if(abs(Objects(idx(i)).Centroid(2)-int16(Objects(im).BoundingBox(4)/2)) <= dist_Thresh & abs(Objects(idx(i)).Centroid(1)-int16(Objects(im).BoundingBox(3)/2)) > 50)
        count=count+1;
        ans = 'i entered'
    elseif(abs(Objects(idx(i)).Centroid(2)-int16(Objects(im).BoundingBox(4)/2)) > dist_Thresh & (Objects(idx(i)).Centroid(2) > 150) & (Objects(idx(i)).Centroid(1) > 400)) 
        flag100= 1;
        ans = 'me too'
    end
end

BW2 = ismember(bwlab, idx);
figure,imshow(BW2);

% check for a 1
% check for a 100 
if(count >=1 )
   ans = 'this is the front' 
    if(flag100 == 1)
        ans = 'this is a 100'
    else
        ans = '1 dollar bill'
    end
end

% report results

% IF no 1 or 100 report an error
if(count < 1)
    ans = 'error please flip over'
end


%------------------- CASE THIS IS THE BACK --------------------------

% If this doesn't have front indicators then this is the back

% get the characters on the bottom-most part and apply filters to feed it
% to the OCR

% segment the characters
if(count < 1)
    % PREPROCESS THE INPUT IMAGE
    % CONVERT IT TO A BINARY IMAGE USING ADAPTIVE THRESHOLDING
    im = imread(str);
    im = rgb2gray(im);
    im = imresize(im,[250 600]);
    medianIM=medfilt2(im,[5, 5]);
    figure,imshow(medianIM);
    bw=im2bw(medianIM,graythresh(medianIM));
    figure,imshow(bw);
    imcomp = imcomplement(bw);
    figure,imshow(imcomp);


    % LABEL THE IMAGE
    bwlab = bwlabel(imcomp);
    Objects = regionprops(bwlab,'BoundingBox','Area','Centroid');

    idx = [];
    Objects
    for i=1:size(Objects,1)
        a = Objects(i).BoundingBox(4)/Objects(i).BoundingBox(3);
        a;
        if(a >= 1.2 & a < 2.6 & Objects(i).Area >= 150 & Objects(i).BoundingBox(3) < 120 & Objects(i).BoundingBox(4) < 100 )
            if(Objects(i).Centroid(1) > 450 & Objects(i).Centroid(2) > 150)
                idx = [idx i];
                idx
            end
        end
    end
    for i=1:size(idx,2)
        x1 = floor(Objects(idx(i)).BoundingBox(2))-2;
        x2 = floor(Objects(idx(i)).BoundingBox(2)+Objects(idx(i)).BoundingBox(4))+2;
        y1 = floor(Objects(idx(i)).BoundingBox(1))-2;
        y2 = floor(Objects(idx(i)).BoundingBox(1)+Objects(idx(i)).BoundingBox(3))+2;
        imwrite(imcomp(x1:x2,y1:y2),strcat(int2str(i),'.tif'))
    end
    BW2 = ismember(bwlab, idx);
    BW2 = imcomplement(BW2);
    figure,imshow(BW2);
end
%------------------ INVOKE TERRACT ON THE CHARS ---------------------
