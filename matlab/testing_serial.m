clc;
% Elimina cualquier puerto abierto
if ~isempty(instrfind)
    fclose(instrfind);
    delete(instrfind);
end

% Detecta el sistema operativo que se est� utilizando.
if ismac
    s = serial('/dev/tty.usbmodem1431');
elseif isunix
    s = serial('/dev/tty.usbmodem1431');
elseif ispc
    s = serial('COM3');
else
    disp('Platforma no soportada');
end

% Configuraci�n de las propiedades del objeto 's'.
set(s,'DataBits',8);
set(s,'StopBits',1);
set(s,'BaudRate',9600);
set(s,'Parity','none');
set(s,'Timeout',5.0);
%set(s,'Terminator','LF');

% Conecta el objeto al dispositivo y muestra su estado
fopen(s);
% pause(5)
if strcmp(s.Status,'open')
    disp('Conexi�n exitosa al puerto especificado.')
else
    disp('Conexi�n fallida al puerto especificado.')
end

% Inicializa el contador.
cont = 1;

% Carga el archivo que contiene las funciones de membres�a y reglas para el
% algoritmo de l�gica difusa.
fzd = readfis('Fuzzy_Logic_Design_2018_v3.fis');

cf(1) = 40; ci(1) = 0;
cf(2) = 40; ci(2) = 0;
phi = pi/2;

flushinput(s);
while true
    tic
    flushinput(s);
    
    % Actualiza las coordenadas en base a los nuevos valores.
    diff_coordx = cf(1)-ci(1);
    diff_coordy = cf(2)-ci(2);
    
    % �ngulo entre el origen y el destino utilizando la funci�n atan2 en el
    % intervalo [-pi, pi]
    delta = atan2(diff_coordy, diff_coordx);
    betarad = radtodeg(delta) - radtodeg(phi);
    if (betarad > 180)
        betarad = betarad - 360;
    else
        if (betarad < -180)
            betarad = betarad + 360;
        end
    end
    
    % Obtiene las lecturas del puerto serie sin el terminador.
    sensorLect = fscanf(s,'%s',30);
    if (strcmp(extractBefore(sensorLect,4),'MSG'))
        % Incrementa el contador. 
        cont = cont + 1;
        % Separa la cadena de caracteres y los guarda en una celda para
        % luego ser convertidos en un arreglo de tipo 'double'.
        dirSL = strsplit(sensorLect,':');
        sensor_val = str2double(dirSL(~isnan(cellfun(@str2double,dirSL))));
        sensor_val(sensor_val > 100) = 100;

        % Ejecuta la l�gica difusa basado en el archivo
        resultFZD = round(evalfis([sensor_val(1:5), betarad, sensor_val(6)],fzd));
        disp(dirSL);
        disp(['Resultado: ', dirSL,  'betarad: ', num2str(betarad), 'Logica: ', num2str(resultFZD)]);
        % Basado en el resultado de la l�gica difusa, realiza la siguiente
        % toma de decisi�n y env�a el comando a tr�ves del puerto serie.
        n_total = round(2.5 * resultFZD);
        if resultFZD < 5 || resultFZD > -5
            fprintf(s,sprintf('%s&%.f','REV', n_total));
            pause(4);
        end
        while n_total ~= 0
            if n_total > 60
                for i=1:fix(n_total/60)
                    n_total = n_total - 60;
                    phi = phi + deg2rad(24);
                    fprintf(s,sprintf('%s&%.f','REV',60));
                    pause(3);
                end        
            else
                if n_total > 0 && n_total <= 60
                    phi = phi + deg2rad(n_total/2.5);
                    fprintf(s,sprintf('%s&%.f','REV', n_total));
                    n_total = n_total - n_total;
                    pause(3);
                else
                    if n_total < -60
                        for i=-1:-1:fix(n_total/60)
                            n_total = n_total + 60;
                            phi = phi - deg2rad(24);
                            fprintf(s,sprintf('%s&%.f','REV',-60));
                            pause(3);
                        end
                    else
                        if n_total >= -60 && n_total < 0
                            phi = phi + deg2rad(n_total/2.5);
                            fprintf(s,sprintf('%s&%.f','REV', n_total));
                            n_total = n_total - n_total;                            
                            pause(3);
                        end
                    end
                end
            end
        end
    end
    toc
end