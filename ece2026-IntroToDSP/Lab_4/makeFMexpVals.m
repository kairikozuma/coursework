function [xx, tt] = makeFMexpVals(sigFMexp, dt)
% Inputs - sigFMexp (struct)
%          Struct with following fields
%          Amp, fc, beta, gamma, t1, t2
%        - dt (double), sampling period

tt = sigFMexp.t1:1/dt:sigFMexp.t2;
psi = 2*pi*(sigFMexp.fc*tt + (sigFMexp.gamma/sigFMexp.beta)*exp(sigFMexp.beta*tt));
xx = sigFMexp.Amp * cos(psi);
end