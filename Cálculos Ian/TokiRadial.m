function [Q, Qq1, Qq2, Qq3] = TokiRadial(teta, r, h) % teta = 0 r = 20 h = 14
tic

%syms h r teta 
syms q1 q2 q3 

L1 = 13.5
L2 = 15
L3 = 20

EqH = r == L2*sin(q2) + L3*sin(q2+q3)
EqR = h == L1 + L2*cos(q2) + L3*cos(q2+q3)
EqT = deg2rad(teta) == q1

Q = solve([EqH EqR EqT], [q1 q2 q3]);

Qq1 = double(rad2deg(Q.q1))
Qq2 = double(rad2deg(Q.q2))
Qq3 = double(rad2deg(Q.q3))
toc