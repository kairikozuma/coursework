%=============================== calibrateM ==============================
%
%  function [M] = calibrateM(qPts, rPts)
%
%
%  INPUT:
%    qPts   - The points in world coordinates.
%    rPts   - The image points (in ray form).
%
%=============================== calibrateM ==============================
function [M] = calibrateM(qPts, rPts)


%--(1) For each world point and image point pair, create the 2-row matrix,
%      and use them to create a master matrix.


%--(2) Perform SVD using the master matrix and extract the projection
%      matrix.  Be careful about rows versus columns ...


end
