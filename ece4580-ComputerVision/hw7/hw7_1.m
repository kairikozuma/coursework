%==========================
%  Name:		hw7_1.m
%
%  Author:		Kairi Kozuma
%==========================


%=============================== findMinima ==============================
%
%  A script to find the location of a function minimum over the planar
%  domain [0,10] x [0,10].  There should be a function called gradF
%  available to the script in order to work.
%
%=============================== findMinima ==============================


%==[1] Initial guess and other parameters.
pos = [0;0];			% Initial estimate of the location of the minimum.

fprintf('Initial guess: (%d, %d)\n', pos(1), pos(2));

c = 10   ;				% Update step factor (sometimes called the timestep).

nsteps = 200 ;			% Number of steps in the gradient descent iterations.

figure(1);
  axis([0 10 0 10]);
  hold on;
  plot(pos(1), pos(2), 'go');

traj = zeros(2,nsteps);

for ii=1:nsteps
  dpos = gradF(pos);
  pos = pos - dpos;
  traj(:,ii) = pos;
  plot(pos(1), pos(2), 'r.');
  drawnow;
end

plot(pos(1), pos(2), 'rx');
fprintf('Final value is (%5.3f, %5.3f).\n\n', pos(1), pos(2));
hold off;
fprintf('Trajectory followed:\n');
for ii=1:nsteps
    fprintf('%d: (%d, %d)\n',ii,traj(1,ii),traj(2,ii));
end
