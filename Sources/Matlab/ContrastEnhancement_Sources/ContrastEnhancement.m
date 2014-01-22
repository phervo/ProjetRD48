% Contrast enhancement function derived from the paper
% "Contrast and Color Enhancement"
% by Mark DeNies, DeNies Video Software. 2012
%
% Copyright (C) 2012 Mark DeNies.
%
% Permission is granted to use this software subject to the condition
% that the author is acknowledged in the derived work and its
% documentation.

function [outImg] = ContrastEnhancement(fname)

    Img = imread(fname);
    HSV = rgb2hsv(Img);
    % amplify the intensity
    V = HSV(:,:,3);
    V = powerLaw(V);
    [maxVal vOut] = enhance(3.0, V, max(V(:)), 4);
    V = maxVal - vOut;
    V = powerLaw(V);
    [maxVal vOut] = enhance(3.0, V, maxVal, 4);
    V = maxVal - vOut;
    HSV(:,:,3) = V ./ maxVal; %put the modified intensity back
    % amplify the saturation
    S = HSV(:,:,2);
    [maxVal sOut] = enhance(2.0, S, max(S(:)), 8);
    S = maxVal - sOut;
    [maxVal sOut] = enhance(2.0, S, maxVal, 8);
    S = maxVal - sOut;
    HSV(:,:,2) = S ./ maxVal; % put the modified saturation back
    
    % reconstruct the RGB image
    outImg = hsv2rgb(HSV);
end

function [oImg] = powerLaw(I)
    maxVal = max(I(:));
    iAvg = mean(I(:));
    power = max(0.6667, min(1.5,(iAvg / (maxVal/2))^2));
    oImg = maxVal * (I./maxVal).^power;
end

function [maxV outImg] = enhance(delta, Img, maxVal, maxStretch)
    outImg = Img;
    [h,w] = size(Img);
    DeltaH = ones(h,w);
    DeltaApply = zeros(h,w);
    Range = maxVal / maxStretch;
    for i=1:128
        sVal = (maxVal*i) / 128; % calculate current threshold
        B = Img > sVal; % Set B = true for all pixels > sVal
        [H nH] = bwlabel(B,8); % Set H = hills, labeled 1:nH
        for j=1:nH
            HList = find(H == j); % fetch current Hill
            pMax = max(Img(HList)); % find it's maximum value
            DeltaMax = min(delta, Range/(pMax - sVal)); % limit the stretching
            % set List = those pixels which can be stretched more: those
            % whose deltaH < delta. Set DeltaApply = the amount to stretch
            % each of them: the remaining delta, or DeltaMax.
            L = HList(find(DeltaH(HList) < delta));
            if ~isempty(L)
                DeltaApply(L) = min(DeltaMax, delta-DeltaH(L));
                outImg(L) = outImg(L) + DeltaApply(L) .* (Img(L) - sVal);
                DeltaH(L) = DeltaH(L) + DeltaApply(L);
            end;
        end;
    end;
    maxV = max(outImg(:));
end