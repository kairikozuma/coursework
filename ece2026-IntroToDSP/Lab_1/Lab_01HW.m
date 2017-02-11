% Record Audio
audiobject = audiorecorder(8000,16,1)
% sampling rate = 8000
% bits = 16
% channel = 1 (2 for stereo)

% Record and stop recording
record(audiobject);
stop(audiobject);

% Records 5 seconds of audio
recordblocking(audiobject, 5);

% Retrieve numeric data
xx = getaudiodata(audioobject);

% Display text
disp('Something');

% Playback audio
play(audiobject);

