clear, clc
syms q1 q2 q3 px py pz

L1 = 12
L2 = L1

EqX = px == (L2+q3)*cos(q2)*cos(q1)
EqY = py == (L2+q3)*cos(q2)*sin(q1)
EqZ = pz == L1 + (L2 + q3)*sin(q2)

[Sol_q1, Sol_q2, Sol_q3] = solve([EqX EqY EqZ], [q1 q2 q3])%, param, cond] , "ReturnConditions",true);
%disp(Sol_q3)

Sol_q1 = simplify(Sol_q1)
Sol_q2 = simplify(Sol_q2)
Sol_q3 = simplify(Sol_q3)
