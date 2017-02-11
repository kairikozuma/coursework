function xs = shortSinus(amp, freq, pha, fs, dur)
% amp = amplitude;
% freq = frequency in Hz
% pha = offset of first peak
% fs = samples per second
% dur = duration in seconds

tt = 0:1/fs:dur;
xs = amp *cos(2*pi*freq*tt + pha);
end