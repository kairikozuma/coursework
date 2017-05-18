%============================= extrinsicCalib ============================
%
%
%  Given a set of image points plus the world coordinates that they came
%  from, and thirdly the intrinsic camera matrix, solve for the extrinsic
%  parameters associated to the camera rig.
%
%
%  function [R, T] = extrinsicCalib(rp, qWpts) 
%
%  Inputs:
%    rp     - Points in the image as homogeneous rays.
%    qWpts  - Points in the world in homogeneous form.
%
%  Outputs:
%    R      - The rotation of the camera frame relative to the world frame.
%    T      - The translation of the camera frame relative to the world frame.
%
%============================= extrinsicCalib ============================
function [R, T] = extrinsicCalib(rp, qWpts)


% Convert image pts (in pixels) to rays (in world length units).

% Define inline function to convert (q,z) pairs into two rows.

% Run for each column in rp and qWpts to construct 12 x 12 matrix.
for i = 1:size(rp,2)
end

% Perform SVD and reconstruct extrinsic parameters (scale by nth root of det).

end
