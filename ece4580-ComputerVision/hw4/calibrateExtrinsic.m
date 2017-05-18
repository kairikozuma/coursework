%========================== calibrateExtrinsic ===========================
%
%  function [R, T] = calibrateExtrinsic(Q)
%
%
%  INPUT:
%    qPts   - The points in world coordinates.
%    rPts   - The image points (in ray form).
%
%  OUTPUT:
%    R      - The rotation matrix (gives camera frame in world frame).
%    T      - The translations (world to camera translation in world frame).
%
%========================== calibrateExtrinsic ===========================
function [R, T] = calibrateExtrinsic(Q)

%--(1) Perform SVD using the master matrix and extract the matrix.  
%      Be careful about rows versus columns ...


%--(2) Normalize the matrix through a scaling by the cube root of
%      the determinante of the top left (3x3) matrix.


%--(3) Extract the rotation matrix and convert to proper frame.


%--(4) Use the rotation matrix to extract the translation.


end
