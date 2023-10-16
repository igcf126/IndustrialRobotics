clear all;
clc;
close all;

l1 = 14.1; % Distancia del piso, a la articulacion del hombro
l2 = 12.6; % Distancia de la articulacion del hombro al codo

l3 = 8.2; % Distancia del codo hacia el paralelo del gripper
l4 = 5; % Distancia del centro de la articulacion al centro del gripper



L1 = Revolute('d', l1, 'a', 0, 'alpha', pi/2);
L2 = Revolute('d', 0, 'a', l2, 'alpha', 0);
L3 = Revolute('d', l4, 'a', l3, 'alpha', 0);


R = SerialLink([L1,L2,L3],'name','Figueroa _A_g_o_s_t_o')



% 
R.plot([0,0,0])
pos = [13,-5,13];
[q] = ARMIKKK(pos(1),pos(2),pos(3));
%[q] = ArmIKK(pos(1),pos(2),pos(3));
%[q] = [0,0,0];
% 
% 
% 
R.plot(q)




% [x,y,z] = sphere;
% 
% surf(x+pos(1),y+pos(2),z+pos(3))


%  x = 10+sin(t)*3;
%  y = 10+cos(t)*3; 
%  plot(x,y)
% 
% % 
% % 
% for i = 1:length(t)
%     pos = [x(i),y(i),z(i)];
%     plot_sphere(pos,0.9,'b');
%     [q] = ARMIKKK(pos(1),pos(2),pos(3));
%     R.plot(q)
% end
