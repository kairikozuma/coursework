 function xx = DTMFdial(keyNames,fs)
   %DTMFDIAL  Create a signal vector of tones that will dial
   %           a DTMF (Touch Tone) telephone system.
   %
   % usage:  xx = DTMFdial(keyNames,fs)
   %  keyNames = vector of CHARACTERS containing valid key names
   %        fs = sampling frequency
   %   xx = signal vector that is the concatenation of DTMF tones.
   %
   TTkeys =  ['1','2','3','A';
               '4','5','6','B';
               '7','8','9','C';
               '*','0','#','D'];
   TTcolTones = [1209,1336,1477,1633];  %-- in Hz
   TTrowTones = [697,770,852,941];
   numKeys = length(keyNames);
   durDualTone = .180;
   tt = linspace(0, durDualTone, round(durDualTone*fs));
   durSilence = .048;
   ts = zeros(1, round(durSilence*fs));
   xx = zeros(1, round((numKeys*(durDualTone + durSilence) + durSilence)*fs));
   n1 = 1;
   for kk=1:numKeys
      %% Determine key number and create cosine values
      [jrow,jcol] = find(keyNames(kk) == TTkeys);
      freq1 = TTcolTones(jcol);
      freq2 = TTrowTones(jrow);
      DTMFsig = cos(2*pi*freq1*tt + rand(1)) + cos(2*pi*freq2*tt + rand(1));
      %% Assign silent vector preceding each signal
      n2 = n1 + length(ts) - 1;
      xx(n1:n2) = ts;
      n2 = n2 + 1;
      n3 = n2 + length(DTMFsig) - 1;
      xx(n2:n3) = xx(n2:n3) + DTMFsig;
      n1 = n3 + 1;
    end
end