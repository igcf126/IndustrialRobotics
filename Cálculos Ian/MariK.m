
function [Q, Qq1, Qq2, Qq3, tha, th2a, thb, th2b] = MariK(px,py,pz)
tic
syms  q1 q2 q3 


A(:,:,1) = [cos(q1), 0,  sin(q1),    0; sin(q1), 0, -cos(q1),    0;      0, 1,        0, 27/2;      0, 0,        0,    1]
m1 =  A(:,:,1)
 
A(:,:,2) = [cos(q2 + pi/2), -sin(q2 + pi/2), 0, 15*cos(q2 + pi/2); sin(q2 + pi/2),  cos(q2 + pi/2), 0, 15*sin(q2 + pi/2);      0,        0, 1,          0;      0,        0, 0,          1]
m2 =  A(:,:,2)
 
A(:,:,3) = [cos(q3), -sin(q3), 0, 20*cos(q3); sin(q3),  cos(q3), 0, 20*sin(q3);      0,        0, 1,          0;      0,        0, 0,          1]
m3 =  A(:,:,3)


T = simplify(simplify(m1*m2)*m3);


    ia = inv(A(:, :, 1));
    ter2 = A(:, :, 2) * A(:, :, 3);
    eq1 = ter2(3,4) == ia (3,1)*px + ia(3,2) *py;
    %q1 = solve(eq1,q1, 'real',true);
    
    ib = inv(A(:, :, 2))*ia;
    ter1 = A(:, :, 3);
    
    eq2 = ter1(2,4) == ib(2,1)*px + ib(2,2)*py + ib(2,3)*pz + ib(2,4)*1;
    eq3 = ter1(1,4) == ib(1,1)*px + ib(1,2)*py + ib(1,3)*pz + ib(1,4)*1;
    
    %assume(q2>0 & q3>0)
    [q2,q3] = solve([eq2,eq3],[q2 q3], 'real', true);
    
    q1 = atan2(-py,-px); %ajustado a mano para dar órdenes de valores negativos
    
    q2a = eval(q2(1));
    q3a = eval(q3(1));

    q2b = eval(q2(2));
    q3b = eval(q3(2));
    
    
     tha = [q1,q2a,q3a];
     thb = [q1,q2b,q3b];
     th2a = rad2deg(tha);
     th2a = wrapTo180(th2a);
     th2b = rad2deg(thb);
     th2b = wrapTo180(th2b);

     toc
    %q2
    %q3
     %%

    Q = [];
    Rot = T(1:3, 1:3);
    Tra = T(1:3, 4);

    EqX = px == (Tra(1));
    EqY = py == (Tra(2));
    EqZ = pz == (Tra(3));

    Eqs =  [EqX; EqY; EqZ];
        
    jointVar = symvar(T);
    
%     assume(q1<0 | q1~=0) %%
    Q = solve([EqX EqY EqZ], jointVar, "Real",true);%, "PrincipalValue",false)
    Qq1 = rad2deg(double(Q.q1));
    Qq2 = rad2deg(double(Q.q2));
    Qq3 = rad2deg(double(Q.q3));

    toc
%     [Q, tha, th2a, thb, th2b] = MariK(5,5,5)