%% Parametros de comunicacion
clc;
% Elimina cualquier puerto abierto
if ~isempty(instrfind)
    fclose(instrfind);
    delete(instrfind);
end

% Detecta el sistema operativo que se está utilizando.
if ismac
    s = serial('/dev/tty.usbserial-AH02QBX1');
elseif isunix
    s = serial('/dev/tty.usbserial-AH02QBX1');
elseif ispc
    s = serial('COM8');
else
    disp('Platforma no soportada');
end

% Configuración de las propiedades del objeto 's'.
set(s,'DataBits',8);
set(s,'StopBits',1);
set(s,'BaudRate',9600);
set(s,'Parity','none');
set(s,'Timeout',10.0);
set(s, 'InputBufferSize', 30)
%set(s,'Terminator','LF');

% Conecta el objeto al dispositivo y muestra su estado
fopen(s);
pause(0.5);
if strcmp(s.Status,'open')
    disp('Conexión exitosa al puerto especificado.')
else
    disp('Conexión fallida al puerto especificado.')
end

s.ReadAsyncMode = 'continuous';
% s.ReadAsyncMode = 'manual';
pause(0.5);

%% Valores iniciales
% Desplazamiento por avance en cm.
A = 20;
% Inicializa el contador.
cont = 1;

% Carga el archivo que contiene las funciones de membresía y reglas para el
% algoritmo de lógica difusa.
fzd = readfis('Fuzzy_Logic_Design_2018_Augusto.fis');

cf(1) = 0; ci(1) = 0;
cf(2) = 150; ci(2) = 0;
phi = pi/2;
ex1 = {'Frontal', 'Diag-Izquierdo', 'Diag-Derecho', 'Izquierdo', 'Derecho'};
%% Código principal
while true
%     tic
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
    
    % Obtiene las lecturas del puerto serie sin el terminador.
    if s.bytesavailable < 18        
        pause(0.1);
    else        
        sensorLect = fscanf(s,'%s');
        disp(sensorLect);
        if (strcmp(extractBefore(sensorLect,4),'MSG'))
            % Incrementa el contador. 
            cont = cont + 1;
            % Separa la cadena de caracteres y los guarda en una celda para
            % luego ser convertidos en un arreglo de tipo 'double'.
            dirSL = strsplit(sensorLect,':');
            sensor_val = str2double(dirSL(~isnan(cellfun(@str2double,dirSL))));
            sensor_val(sensor_val > 100) = 100;                

            % Ejecuta la lógica difusa basado en el archivo
            resultFZD = round(evalfis([sensor_val(4), sensor_val(2), sensor_val(1), ...
                                   sensor_val(3), sensor_val(5), betarad, sensor_val(6)],fzd));
            if resultFZD <= 15 && resultFZD >= -15                    
                ci(1) = round(ci(1) + A*cos(phi));
                ci(2) = round(ci(2) + A*sin(phi));
            else
                phi = phi + deg2rad(resultFZD);
            end
            ex2 = cell(1,5);
            for i = 1:length(sensor_val(1:5))
                if(sensor_val(i) >= 55)
                    ex2{i} = 'far';
                else
                    ex2{i} = 'near';
                end
            end
            % Imprime los datos por iteracciòn.
            fprintf('Resultado N°%d: \n', cont);
            disp(cellfun(@(a,b)[a(:,1:end) ': ' b(:,1:end)], ex1, ex2, 'uni', 0));
            fprintf('Sensorica: [%d;%d;%d;%d;%d;%d;%.2f] \n', ...
                sensor_val(4), sensor_val(2), sensor_val(1), ...
                sensor_val(3), sensor_val(5), betarad, sensor_val(6));
            fprintf('Araña: %d | Beta: %d | Logica: %d\n', round(radtodeg(phi)), betarad, resultFZD);
            disp('##################################################');
            % Basado en el resultado de la lógica difusa, realiza la siguiente
            % toma de decisión y envía el comando a tráves del puerto serie.            
            flushinput(s);
            fprintf(s,sprintf('<%s&%d>','REV', resultFZD));                
            pause(0.1);
            sensorLect = fscanf(s,'%s', 6);
            if strcmp(sensorLect,'|ACK|')
                disp('Mensaje recibido.');
                disp('##################################################');
                flushinput(s);
            end
        end
    end
%     toc
end