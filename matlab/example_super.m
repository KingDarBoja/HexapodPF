cont = 1;

flc1 = readfis('FLC1_hexapod.fis');
flc2 = readfis('FLC2_hexapod.fis');
flc3 = readfis('FLC3_hexapod.fis');
flc4 = readfis('FLC4_hexapod.fis');
flc_s = readfis('FLCS_hexapod.fis');

cf(1) = 100; ci(1) = 0;
cf(2) = 50; ci(2) = 0;
phi = pi/2;

% Actualiza las coordenadas en base a los nuevos valores.
diff_coordx = cf(1)-ci(1);
diff_coordy = cf(2)-ci(2);

% Ángulo entre el origen y el destino utilizando la función atan2 en el
% intervalo [-pi, pi]
delta = atan2(diff_coordy, diff_coordx);
betarad = round(radtodeg(delta)) - round(radtodeg(phi));
if (betarad > 180)
    betarad = betarad - 360;
else
    if (betarad < -180)
        betarad = betarad + 360;
    end
end

% Separa la cadena de caracteres y los guarda en una celda para
% luego ser convertidos en un arreglo de tipo 'double'.
sensor_val = [40, 40, 30, 30, 100, 45];

res_super = evalfis([sensor_val(1:5), betarad], flc_s);
res_1 = evalfis(sensor_val(1:5), flc1);
res_2 = evalfis(sensor_val(1:5), flc2);
res_3 = evalfis(betarad, flc3);
res_4 = evalfis(sensor_val(1:5), flc4);
% Ejecuta la lógica difusa basado en el archivo
% resultFZD = round(evalfis([sensor_val(4), sensor_val(2), sensor_val(1), ...
%                           sensor_val(3), sensor_val(5), betarad, sensor_val(6)],fzd));
disp(['Resultado: ', num2str(sensor_val), ' Logica super: ', num2str(res_super)]);
disp(['Resultado: ', num2str(sensor_val), ' Logica obstacle avoidance: ', num2str(res_1)]);
disp(['Resultado: ', num2str(sensor_val), ' Logica wall following: ', num2str(res_2)]);
disp(['Resultado: ', num2str(betarad), ' Logica goal reaching: ', num2str(res_3)]);
disp(['Resultado: ', num2str(sensor_val), ' Logica emergency: ', num2str(res_4)]);