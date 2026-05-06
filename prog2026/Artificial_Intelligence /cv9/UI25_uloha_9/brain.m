clear; clc;

fis = mamfis('Name','brain');

%% INPUT 1: pocet aut na zelenej
fis = addInput(fis,[0 40],'Name','cars_green');

fis = addMF(fis,'cars_green','trapmf',[0 0 3 7],'Name','low');
fis = addMF(fis,'cars_green','trimf',[4 9 14],'Name','medium');
fis = addMF(fis,'cars_green','trapmf',[11 16 40 40],'Name','high');

%% INPUT 2: pocet aut na cervenej
fis = addInput(fis,[0 120],'Name','cars_red');

fis = addMF(fis,'cars_red','trapmf',[0 0 8 16],'Name','low');
fis = addMF(fis,'cars_red','trimf',[12 30 48],'Name','medium');
fis = addMF(fis,'cars_red','trapmf',[40 55 120 120],'Name','high');

%% OUTPUT: dlzka zelenej
fis = addOutput(fis,[26 30],'Name','green_duration');

fis = addMF(fis,'green_duration','trapmf',[26 26 27 28],'Name','short');
fis = addMF(fis,'green_duration','trimf',[27 28.5 30],'Name','medium');
fis = addMF(fis,'green_duration','trapmf',[28 29 30 30],'Name','long');

%% PRAVIDLA
% Format:
% [cars_green cars_red green_duration weight AND]
%
% cars_green:
% 1 = low
% 2 = medium
% 3 = high
%
% cars_red:
% 1 = low
% 2 = medium
% 3 = high
%
% green_duration:
% 1 = short
% 2 = medium
% 3 = long

rules = [
    1 1 2 1 1;   % low / low    -> medium
    1 2 2 1 1;   % low / medium -> medium
    1 3 2 1 1;   % low / high   -> medium

    2 1 3 1 1;   % medium / low    -> long
    2 2 3 1 1;   % medium / medium -> long
    2 3 3 1 1;   % medium / high   -> long

    3 1 3 1 1;   % high / low    -> long
    3 2 3 1 1;   % high / medium -> long
    3 3 3 1 1;   % high / high   -> long
];

fis = addRule(fis,rules);

%% Ulozenie fuzzy systemu
writeFIS(fis,'brain');

disp('Fuzzy system f.fis bol vytvoreny.');