%% read the input image
close all,clear all;
I=im2double(imread('input.png'));

%% get edge map
% Canny edge detector is used here. Other edge detectors can also be used
eth=0.1; % thershold for canny edge detector
edgeMap=edge(rgb2gray(I),'canny',eth,1);

%% estimate the defocus map
std=1;
lambda=0.001;
maxBlur=3;
tic; % about 1 mins for a 800x600 image
[sDMap, fDmap] = defocusEstimation(I,edgeMap,std,lambda,maxBlur);
toc;
imshow(fDmap,[0 maxBlur]);
