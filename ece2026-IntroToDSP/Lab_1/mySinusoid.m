% Creates a sinusoid

function xs = mySinusoid(amp, freq, pha, fs, tsta, tend)
% amp = amplitude
% freq = frequency in cycles per second
% pha = phase, time offset of first peak
% fs = number of sample values per second
% tsta = starting time in sec
% tend = ending time in sec
tt = tsta:1/fs:tend; % time indices
xs = amp*cos(freq*2*pi*tt+pha);
end