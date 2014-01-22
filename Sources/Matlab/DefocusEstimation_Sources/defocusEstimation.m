
function [sparseDMap, fullDMap] = defocusEstimation(I,edgeMap,std,lambda,maxBlur)
% This function calculate the defocus map from a single defoused image
% input:
%       I             -       the input defocused image
%       edgeMap       -       the edge map of the input image
%       std           -       the standard devitation reblur gaussian1, typically std=[0.8:1]
%       lambda        -       the weight of the constraint of sparse defocus map when perform defocus map propagation
%       maxBlur       -       the max blur in the image
% output:
%       sparseDMap    -     sparse defocus map
%       fullDMap      -     full defocus map
%
% Shaojie Zhuo @ 2011

std1 = std;
std2 = 1.5*std;

%% Calculate gaussian gradient magnitude of the original and reblur images
gI=rgb2gray(I);
w1 = (2*ceil(2* std1))+1;
[X1 Y1] = meshgrid([-w1:w1]);
gx1 = g1x(X1, Y1, std1);
gy1 = g1y(X1, Y1, std1);

gimx1 = imfilter(gI, gx1,'replicate','conv','same');
gimy1 = imfilter(gI, gy1,'replicate','conv','same');
mg1=sqrt(gimx1.^2+gimy1.^2);

w2 = (2*ceil(2 * std2))+1;
[X2 Y2] = meshgrid([-w2:w2]);
gx2 = g1x(X2, Y2, std2);
gy2 = g1y(X2, Y2, std2);

gimx2 = imfilter(gI, gx2,'replicate','conv','same');
gimy2 = imfilter(gI, gy2,'replicate','conv','same');
mg2=sqrt(gimx2.^2+gimy2.^2);

gRatio=mg1./mg2;

%% Generate the sparse defocus map from the gradient magnitude ratio
idx=find(edgeMap~=0);
sparseDMap=zeros(size(gI));

for jj=1:length(idx)
    if (gRatio(idx(jj))> 1.01 && gRatio(idx(jj))<=(std2/std1))
        sparseDMap(idx(jj))=sqrt((gRatio(idx(jj)).^2*std1^2-std2^2)/(1-gRatio(idx(jj)).^2));
    end
end

%% Filter the spase defocus map to remove noise
[h,w,c]=size(I);
sparseDMap(sparseDMap>maxBlur)=maxBlur;
sd=5; % spatial sigma
sr=0.1*maxBlur; % range sigma
[sparseDMap ~]=sparseBilateralFilter(sparseDMap,I,sd,sr);

%% Propagate the spase defocus map based on color similarities to get full defocus map
sizeI=w*h;
constsMap=sparseDMap>0.0001;
L=getLaplacian(I,1);
D=spdiags(constsMap(:),0,sizeI,sizeI);

% try pcg if you get out of memory error using the \ solver
% x=pcg(L+lambda*D,lambda*D*sparseDMap(:),1e-6,1000); 
x=(L+lambda*D)\(lambda*D*sparseDMap(:));
fullDMap=reshape(x,h,w);

