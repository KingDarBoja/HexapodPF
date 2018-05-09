%% Parametros de comunicacion
clc;
% Elimina cualquier puerto abierto
if ~isempty(instrfind)
    fclose(instrfind);
    delete(instrfind);
end

% Detecta el sistema operativo que se est� utilizando.
if ismac
    s = serial('/dev/tty.usbserial-AH02QBX1');
elseif isunix
    s = serial('/dev/tty.usbserial-AH02QBX1');
elseif ispc
    s = serial('COM8');
else
    disp('Platforma no soportada');
end

% Configuraci�n de las propiedades del objeto 's'.
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
    disp('Conexi�n exitosa al puerto especificado.')
else
    disp('Conexi�n fallida al puerto especificado.')
end

s.ReadAsyncMode = 'continuous';
% s.ReadAsyncMode = 'manual';
pause(0.5);

%% Valores iniciales

% Inicializa el contador.
cont = 1;

% Carga el archivo que contiene las funciones de membres�a y reglas para el
% algoritmo de l�gica difusa.
fzd = readfis('Fuzzy_Logic_Design_2018_Augusto.fis');

cf(1) = 100; ci(1) = 0;
cf(2) = 50; ci(2) = 0;
phi = pi/2;

%% C�digo principal
while true
%     tic
    % Actualiza las coordenadas en base a los nuevos valores.
    diff_coordx = cf(1)-ci(1);
    diff_coordy = cf(2)-ci(2);
    
    % �ngulo entre el origen y el destino utilizando la funci�n atan2 en el
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
        fprintf('.');
        pause(0.1);
    else
        flushinput(s);
        sensorLect = fscanf(s,'%s', 30);
        if (strcmp(extractBefore(sensorLect,4),'MSG'))
            % Incrementa el contador. 
            cont = cont + 1;
            % Separa la cadena de caracteres y los guarda en una celda para
            % luego ser convertidos en un arreglo de tipo 'double'.
            dirSL = strsplit(sensorLect,':');
            sensor_val = str2double(dirSL(~isnan(cellfun(@str2double,dirSL))));
            sensor_val(sensor_val > 100) = 100;

            % Ejecuta la l�gica difusa basado en el archivo
            resultFZD = round(evalfis([sensor_val(4), sensor_val(2), sensor_val(1), ...
                                       sensor_val(3), sensor_val(5), betarad, sensor_val(6)],fzd));
            disp(['Resultado: ', dirSL,  'betarad: ', num2str(betarad), 'Logica: ', num2str(resultFZD)]);
            % Basado en el resultado de la l�gica difusa, realiza la siguiente
            % toma de decisi�n y env�a el comando a tr�ves del puerto serie.            
%             flushinput(s);
            fprintf(s,sprintf('<%s&%d>','REV', resultFZD));
            pause(0.1);
            sensorLect = fscanf(s,'%s', 6);
            if strcmp(sensorLect,'|ACK|')
                disp('Recibido');
            end            
        end
    end
%     toc
end