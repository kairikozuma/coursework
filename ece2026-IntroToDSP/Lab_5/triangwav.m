%% 2.6 Triangular Wave
function coeff = triangwav(k)
if (k == 0)
    coeff = 0;
elseif (mod(k, 2) == 1)
    coeff = -2/(pi^2*k^2);
elseif (mod(k, 2) == 0)
    coeff = 1/2;
else
    coeff = -1;
end
end