syms q1 q2 q3 px py pz

L1 = 12
L2 = L1

EqX = px == L1*cos(q1) + L2*cos(q1+q2)
EqY = py == L1*sin(q1) + L2*sin(q1+q2)
EqZ = pz == L1 - q3

[Sol_q1 Sol_q2 Sol_q3] = solve([EqX EqY EqZ], [q1 q2 q3]);

Sol_q1 = simplify(Sol_q1)
Sol_q2 = simplify(Sol_q2)
Sol_q3 = simplify(Sol_q3)
