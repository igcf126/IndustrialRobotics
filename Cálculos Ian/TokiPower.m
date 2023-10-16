function [Q, Qq1, Qq2, Qq3] = TokiPower(px, py, pz)
tic

syms q1 q2 q3

r = sqrt(px^2+py^2);
h = pz;
teta = atan2(-py,-px);

L1 = 13.5;
L2 = 15;
L3 = 20;

EqH = r == L2*sin(q2) + L3*sin(q2+q3);
EqR = h == L1 + L2*cos(q2) + L3*cos(q2+q3);
EqT = teta == q1;

Q = solve([EqH EqR EqT], [q1 q2 q3]);

Qq1 = double(rad2deg(Q.q1))
Qq2 = double(rad2deg(Q.q2))
Qq3 = double(rad2deg(Q.q3))

toc