function [A, T, Q, Rot, Tra] = DH(Param, px, py, pz, az)
    %syms px py pz

    cant = length(Param(:,1));
    disp(cant);
    for i = 1:cant
        joint = Param(i,:);
        theta = joint(1);
        d = joint(2);
        a = joint(3);
        alpha = joint(4);

        disp(joint)
        A(:, :, i) = [cos(theta) -cos(alpha)*sin(theta) sin(alpha)*sin(theta) a*cos(theta);
            sin(theta) cos(alpha)*cos(theta) -sin(alpha)*cos(theta) a*sin(theta);
            0 sin(alpha) cos(alpha) d;
            0 0 0 1];
    end
    disp(A)
    T=1;
    for i = 1:cant
        T=simplify(T*A(:,:,i));
    end
    
    disp(T);

    Q = [];
    Rot = T(1:3, 1:3);
    Tra = T(1:3, 4);

    EqX = px == (Tra(1));
    EqY = py == (Tra(2));
    EqZ = pz == (Tra(3));

    Eqs =  [EqX; EqY; EqZ];
    % n, s, a
    %EqAux = az == Rot(2,3);
    %EqAux = az == Rot(3,3);
    
    EqAux = az == Rot(3,1);
    
    jointVar = symvar(T);
    
    
%     Q = solve([EqX EqY EqZ], jointVar, "Real",true)%, "PrincipalValue",false)
    
    %Q = simplify(Q.q1)


end
    %for i