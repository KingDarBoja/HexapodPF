clc;
% Elimina cualquier puerto abierto
if ~isempty(instrfind)
    fclose(instrfind);
    delete(instrfind);
end

% Detecta el sistema operativo que se está utilizando.
if ismac
    s = serial('/dev/tty.usbmodem1431');
elseif isunix
    s = serial('/dev/tty.usbmodem1431');
elseif ispc
    s = serial('COM3');
else
    disp('Platforma no soportada');
end

% Configuración de las propiedades del objeto 's'.
set(s,'DataBits',8);
set(s,'StopBits',1);
set(s,'BaudRate',115200);
set(s,'Parity','none');

% Conecta el objeto al dispositivo y muestra su estado
fopen(s);
pause(5)
if strcmp(s.Status,'open')
    disp('Conexión exitosa al puerto especificado.')
else
    disp('Conexión fallida al puerto especificado.')
end

while true
    sensorLect = fgetl(s);
    disp(sensorLect);
end