function sigBeatSum = sum2BeatStruct(sigBeatIn)
% Description: Adds vector of time and signal values as well as the complex
%               amplitudes to the signal structure based on fields already present.
% 
% Fields present:
% Amp = amplitude
% fc = central frequency
% fDelt = modulation frequency
% phic = central phase
% phiDelt = modulation phase
% t1 = starting time
% t2 = ending time
% fs = sampling rate

% Output fields added:
% X1 = complex amplitude 1
% X2 = complex amplitude 2
% f1 = frequency 1
% f2 = frequency 2
% times = time vector
% values = signal values for corresponding time interval
sigBeatSum = sigBeatIn;
f1 = sigBeatIn.fc - sigBeatIn.fDelt;
f2 = sigBeatIn.fc + sigBeatIn.fDelt;
X1 = sigBeatIn.Amp*(1/2)*exp(j*(sigBeatIn.phic - sigBeatIn.phiDelt));
X2 = sigBeatIn.Amp*(1/2)*exp(j*(sigBeatIn.phic + sigBeatIn.phiDelt));
sigBeatSum.f1 = f1;
sigBeatSum.f2 = f2;
sigBeatSum.X1 = X1;
sigBeatSum.X2 = X2;
tt = sigBeatIn.t1:1/sigBeatIn.fs:sigBeatIn.t2;
xx = real(X1*exp(j*2*pi*f1*tt) + X2*exp(j*2*pi*f2*tt));
sigBeatSum.values = xx;
sigBeatSum.times = tt;
end