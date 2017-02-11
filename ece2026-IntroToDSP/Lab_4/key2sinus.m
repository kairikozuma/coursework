function [xx, tt] = key2sinus(keynum, amp, phase, fsamp, dur)
% KEY2SINUS Produce a sinusoidal waveform corresonpoding to a given piano
%       key number
% usage: xx = key2sinus(keynum, amp, phase, fsamp, dur);
%
% xx = output of sinusoidal waveform
% tt = vector of sampling times
% keynum = piano keyboard number of desired note
% amp, phase, sinusoid parameters
% fsamp = sampling frequency (800,11025, 22050)
% dur = duration in seconds of output note
tt = 0:(1/fsamp):dur;
freqKey = 440*2^((keynum-49)/12);
Xphasor = amp*exp(j*phase);
xx = real(Xphasor*exp(j*2*pi*freqKey*tt));
end