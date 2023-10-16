function [th] = ARMIKKK(px,py,pz)


%% base roja 10 
% 7.6 piso a base roja 4.6

l1 = 14.6;
l2 = 12.6;
l3 = 8.2;
% Robot angular

syms  q1 q2 q3 


m1 = [
    cos(q1), -cosd(90) * sind(q1), sind(90) * sin(q1), 0 * cos(q1);
    sin(q1), cosd(90) * cosd(q1), -sind(90) * cos(q1), 0 * sin(q1);
    0, sind(90), cosd(90), l1;
    0, 0, 0, 1];

m2 = [
    cos(q2), -cosd(0) * sin(q2), sind(0) * sin(q2), l2 * cos(q2);
    sin(q2), cosd(0) * cos(q2), -sind(0) * cos(q2), l2 * sin(q2);
    0, sind(0), cosd(0), 0;
    0, 0, 0, 1 ];

m3 = [
    cos(q3), -cosd(0) * sin(q3), sind(0) * sin(q3), l3 * cos(q3);
    sin(q3), cosd(0) * cos(q3), -sind(0) * cos(q3), l3 * sin(q3);
    0, sind(0), cosd(0), 0;
    0, 0, 0, 1 ];

T = simplify(simplify(m1*m2)*m3);


eq1 = px == expand(T(1,4));
eq2 = py == expand(T(2,4));
eq3 = pz == expand(T(3,4));


ia = inv(m1);
ter2 = m2 * m3;
eq1 = ter2(3,4) == ia (3,1)*px + ia(3,2) *py;
%q1 = solve(eq1,q1, 'real',true);

ib = inv(m2)*ia;
ter1 = m3;

eq2 = ter1(2,4) == ib(2,1)*px + ib(2,2)*py + ib(2,3)*pz + ib(2,4)*1;
eq3 = ter1(1,4) == ib(1,1)*px + ib(1,2)*py + ib(1,3)*pz + ib(1,4)*1;

[q2,q3] = solve([eq2,eq3],[q2 q3], 'real', true);

q1 = atan2(py,px);

q2 = eval(q2(1));
q3 = eval(q3(1));


th = [q1,q2,q3]
th2 = rad2deg(th)


% q1 = atan2(py,px)

% eq2 = subs(T2(1,4),q1,sol_q1);
% eq3 = subs(T2(2,4),q1,sol_q1);
% [q2,q3] = solve([eq1,eq2,eq3],[q2,q3])

end
    
