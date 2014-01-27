
function [Ib, Id] = sparseBilateralFilter(I,G,sd,sr)
% This function perform the bilateral filtering to refind the sparse defocs
% map
%
% Shaojie Zhuo @ 2011
[h,w,c] = size(I);
[hg,hg,cg]=size(G);
oriI = I;
Ib = zeros(h,w,c);
% Gaussian spatial domain filter
wr = ceil(sd*4)+1;
f = fspecial('gaussian', wr, sd);
pnum = (wr-1)/2;
pG = padarray(G,[pnum pnum],'replicate','both');
for cc=1:c
    pI = padarray(I(:,:,cc),[pnum pnum],'replicate','both');
    for jj = pnum+1:(pnum+h)
        for kk = pnum+1:(pnum+w)
            if(I(jj-pnum,kk-pnum)>0.0001)
                % Get a local neighborhood
                winI = pI(jj-pnum:jj+pnum,kk-pnum:kk+pnum);
                winG=pG(jj-pnum:jj+pnum,kk-pnum:kk+pnum,1:cg);
                winGW = sum(exp(-(winG - winG(pnum+1,pnum+1)).^2/(sr^2*2)),3);
                newW = winGW.*f.*exp(-winI);
                index=find(winI>0.0001);
                t = sum(sum(newW(index)));
                Ib(jj-pnum,kk-pnum,cc) = sum(sum(winI(index).*newW(index)));
                if(t>0)
                    Ib(jj-pnum,kk-pnum,cc) = Ib(jj-pnum,kk-pnum,cc)/t;
                end
            end
        end
    end
end

Id = oriI ./ Ib;
