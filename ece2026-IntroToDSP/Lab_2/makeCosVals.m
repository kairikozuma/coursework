function sinu = makeCosVals(sinstru, dur, ts, dt)
% Generates a sinusoid from given values
% Inputs: (4) sinstru (struct) - frequency and amplitude as its fields
%             dur     (double) - duration of sinusoid
%             ts      (double) - start time
%             dt      (double) - time interval of points
% Outputs:(1) cosval  (struct) - has both fields of sinstru and 2 new
%                                fields: vector of values and time values

% Retrieve frequency and amplitude from structure
freq = sinstru.freq;
amp = sinstru.complexAmp;

% Initialize sinu struct
sinu.freq = freq;
sinu.complexAmp = amp;

% If dt is greater than 1/32 seconds, set it to 1/32 seconds
if (dt > 1/32)
    dt = 1/32;
end

% Create time values
tt = ts:dt:ts + dur;

% Generate cosine values
cosval = amp*cos(2*pi*freq*tt);

% Assign cosine values and time values to output struct
sinu.times = tt;
sinu.values = cosval;
end

