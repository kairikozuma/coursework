   function sigOut = addCosVals(cosIn, dur, tstart, dt)
% ADDCOSVALS Syntehsize a signal from sum of sinusoids
%
% sigOut = addCosVals(cosIn, dur, tstart, dt)
% Inputs (4): (struct) - vector of structures with following fields:
%                        cosIn.freq = frequiency in Hz
%                        cosIn.complexAmp = complex amplitude of cosine
%             (double) - total time duration of all cosines
%             (double) - start time
%             (double) - time increment for time vector
% Outputs(1): (struct) - struct of signal values with two fields:
%                        sigOut.values = vector of signal values
%                        sigOut.times = vector of times, for time axis
%
% Note: 32 samples per second of smallest period taken

% Create vector of zeros for placeholder

% Generate values for each sinusoid and store them in vector
for k = 1:length(cosIn)
    vec(k) = makeCosVals(cosIn(k), dur, tstart, dt);
end

cossum = sum(cat(1, vec(:).values),1);
times = vec(1).times;

% Assign values to struct
sigOut.values = cossum;
sigOut.times = times;

end


