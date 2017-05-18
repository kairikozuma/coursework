%============================= calibrateFull ============================
%
%  function [Psi, D] = calibrateM(qPts, rPts)
%
%
%  INPUT:
%    qPts   - The points in world coordinates.
%    rPts   - The image points (in ray form).
%
%  OUTPUT:
%    Psi    - The intrinsic camera matrix.
%    D      - The extrinsic matrix (world to camera transformation).
%
%============================= calibrateFull ============================
function [Psi, D] = calibrateM(qPts, rPts)

% NOTE: SOME OF THE CODE FROM PREVIOUS PROBLEM CAN BE RECYCLED.

%--(1) For each world point and image point pair, create the 2-row matrix,
%      and use them to create a master matrix.


%--(2) Perform SVD using the master matrix and extract the projection
%      matrix.  Be careful about rows versus columns ...


%--(3) Grab the leftmost 3x3 matrix and use QR decomposition plus
%      some tricks to extract Psi and R.  Make sure to test out that
%      R has positive determinant and follows right hand rule.


%--(4) Use Psi to recover T.  Then everything is ready to be returned.


end
