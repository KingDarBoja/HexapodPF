function varargout = GUI_2018(varargin)
% GUI_2018 MATLAB code for GUI_2018.fig
%      GUI_2018, by itself, creates a new GUI_2018 or raises the existing
%      singleton*.
%
%      H = GUI_2018 returns the handle to a new GUI_2018 or the handle to
%      the existing singleton*.
%
%      GUI_2018('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in GUI_2018.M with the given input arguments.
%
%      GUI_2018('Property','Value',...) creates a new GUI_2018 or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI_2018 before GUI_2018_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to GUI_2018_OpeningFcn via varargin.
%
%      *See GUI_2018 Options on GUIDE's Tools menu.  Choose "GUI_2018 allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help GUI_2018

% Last Modified by GUIDE v2.5 19-Mar-2018 12:05:51

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @GUI_2018_OpeningFcn, ...
                   'gui_OutputFcn',  @GUI_2018_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before GUI_2018 is made visible.
function GUI_2018_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to GUI_2018 (see VARARGIN)

% Choose default command line output for GUI_2018
handles.output = hObject;


% Update handles structure
guidata(hObject, handles);

% UIWAIT makes GUI_2018 wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = GUI_2018_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;



function cfy_Callback(hObject, eventdata, handles)
% hObject    handle to cfy (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of cfy as text
%        str2double(get(hObject,'String')) returns contents of cfy as a double


% --- Executes during object creation, after setting all properties.
function cfy_CreateFcn(hObject, eventdata, handles)
% hObject    handle to cfy (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function cfx_Callback(hObject, eventdata, handles)
% hObject    handle to cfx (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of cfx as text
%        str2double(get(hObject,'String')) returns contents of cfx as a double


% --- Executes during object creation, after setting all properties.
function cfx_CreateFcn(hObject, eventdata, handles)
% hObject    handle to cfx (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in Iniciar.
function Iniciar_Callback(hObject, eventdata, handles)
% hObject    handle to Iniciar (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% Clean ports caché.

% Limpia la consola
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
    s = serial('COM3');
else
    disp('Platforma no soportada');
end

% Configuración de las propiedades del objeto 's'.
set(s,'DataBits',8);
set(s,'StopBits',1);
set(s,'BaudRate',9600);
set(s,'Parity','none');
set(s,'Timeout',10.0);
% set(s,'Terminator','LF');
set(s, 'InputBufferSize', 30);

% Conecta el objeto al dispositivo y muestra su estado
fopen(s);
pause(0.5);
if strcmp(s.Status,'open')
    disp('Conexión exitosa al puerto especificado.')
else
    disp('Conexión fallida al puerto especificado.')
end
% Coordenadas finales (meta).
cf = [str2double(get(handles.cfx,'String')), ...
    str2double(get(handles.cfy,'String'))];
% Coordenadas iniciales (partida).
ci = [str2double(get(handles.cix,'String')), ...
    str2double(get(handles.ciy,'String'))];
% Ángulo inicial del robot.
phi = pi/2;
% Desplazamiento por avance en cm.
A = 20;
% Umbral de error permitido en cm.
err_perm = 20;

% Inicializa el contador.
cont = 1;

% Obtiene el estado del widget 'checkbox1'
checkbox1Value = get(handles.checkbox1, 'value');
diff_coordx = cf(1)-ci(1);
diff_coordy = cf(2)-ci(2);

% Inizializa las variables que corresponden a las coordenadas del robot y
% los obstaculos para dibujarlos en el mapa.
coordx = ci(1);
coordy = ci(2);
mapRx(cont) = 0;
mapRy(cont) = 0;
mapLx(cont) = 0;
mapLy(cont) = 0;
phimat = radtodeg(phi);
betamat(cont) = 0;
ex1 = {'Frontal', 'Diag-Izquierdo', 'Diag-Derecho', 'Izquierdo', 'Derecho'};
% Carga el archivo que contiene las funciones de membresía y reglas para el
% algoritmo de lógica difusa.
fzd = readfis('Fuzzy_Logic_Design_2018_Augusto.fis');

s.ReadAsyncMode = 'continuous';
% s.ReadAsyncMode = 'manual';
pause(0.5);

% Estado del movimiento.
mov_frontal(1) = 'S'; % Movimiento previo.
mov_frontal(2) = 'S'; % Movimiento actual.
sensor_hist = [[0 0]; [0 0]];

% Ciclo para revisar si el robot llegó a la meta o si se ha detenido el
% programa utilizando el estado del checkbox.
while ((abs(diff_coordx) > err_perm || abs(diff_coordy) > err_perm) ...
        && checkbox1Value == 0)
    % disp(cont);
    % flushinput(s);
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
        % disp('.');
        pause(0.1);
    else        
        try
            % Limpia el buffer de entrada del puerto serial.
            sensorLect = fscanf(s, '%s', 30);
            % disp(sensorLect);
            if (strcmp(extractBefore(sensorLect,4),'MSG'))
                % Incrementa el contador. 
                cont = cont + 1;
                % Separa la cadena de caracteres y los guarda en una celda para
                % luego ser convertidos en un arreglo de tipo 'double'.
                dirSL = strsplit(sensorLect,':');
                sensor_val = str2double(dirSL(~isnan(cellfun(@str2double,dirSL))));
                sensor_val(sensor_val > 100) = 100;

                sensor_hist(2) = sensor_val(4);
                sensor_hist(4) = sensor_val(5);
                if mov_frontal(1) == 'F' && mov_frontal(2) == 'F'
                    fprintf('¡Verifica los sensores! \n ---- Anterior --- \n');
                    fprintf('Sensor Izquierdo: %d | Sensor Derecho: %d \n', ...
                        sensor_hist(1), sensor_hist(3));
                    fprintf('--- Actual --- \n Sensor Izquierdo: %d | Sensor Derecho: %d \n', ...
                        sensor_hist(2), sensor_hist(4));
                    if sensor_hist(2) > sensor_hist(1) + 30
                        flushinput(s);
                        fprintf(s,sprintf('<%s&%d>','REV', 0));
                        pause(0.1);
                        fprintf(s,sprintf('<%s&%d>','REV', 80));
                        pause(0.1);
                        fprintf(s,sprintf('<%s&%d>','REV', 0));
                        phi = phi + deg2rad(90);
                        mov_frontal(2) = 'T';
                        disp('Lo hizo a la derecha');
                    else
                        if sensor_hist(4) > sensor_hist(3) + 30
                            flushinput(s);
                            fprintf(s,sprintf('<%s&%d>','REV', 0));
                            pause(0.1);
                            fprintf(s,sprintf('<%s&%d>','REV', -80));
                            pause(0.1);
                            fprintf(s,sprintf('<%s&%d>','REV', 0));
                            pause(0.1);
                            phi = phi + deg2rad(-90);
                            mov_frontal(2) = 'T';
                            disp('Lo hizo a la izquierda');
                        else
                            sensor_hist(1) = sensor_hist(2);
                            sensor_hist(3) = sensor_hist(4);
                            mov_frontal(1) = 'T';
                            disp('No hizo nada');
                        end
                    end
                    flushinput(s);
                else
                    % Ejecuta la lógica difusa basado en el archivo
                    resultFZD = round(evalfis([sensor_val(4), sensor_val(2), sensor_val(1), ...
                                           sensor_val(3), sensor_val(5), betarad],fzd));
                    mov_frontal(1) = mov_frontal(2);                
                    if resultFZD <= 15 && resultFZD >= -15
                        ci(1) = round(ci(1) + A*cos(phi));
                        ci(2) = round(ci(2) + A*sin(phi));
                        mov_frontal(2) = 'F';
                    else
                        phi = phi + deg2rad(resultFZD);
                        mov_frontal(2) = 'T';
                    end
                    sensor_hist(1) = sensor_hist(2);
                    sensor_hist(3) = sensor_hist(4);
                    % Impresión de valores de manera verbal.
                    ex2 = cell(1,5);
                    for i = 1:length(sensor_val(1:5))
                        if(sensor_val(i) >= 35)
                            ex2{i} = 'far';
                        else
                            ex2{i} = 'near';
                        end
                    end
                    % Imprime los datos por iteracciòn.
                    fprintf('Resultado N°%d: \n', cont);
                    disp(cellfun(@(a,b)[a(:,1:end) ': ' b(:,1:end)], ex1, ex2, 'uni', 0));
                    fprintf('Sensorica: [%d;%d;%d;%d;%d;%d] \n', ...
                        sensor_val(4), sensor_val(2), sensor_val(1), ...
                        sensor_val(3), sensor_val(5), betarad);
                    fprintf('Araña: %d | Beta: %d | Logica: %d | Elevación: %d\n', ...
                        round(radtodeg(phi)), betarad, resultFZD, round(sensor_val(6)));
                    fprintf('Posiciòn: X - %d, Y - %d \n', ci(1), ci(2));
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
                    end
                end
            end
            coordx(cont) = ci(1);
            coordy(cont) = ci(2);
            % Realiza los cálculos de la ubicación de los objetos.
            phimat(cont) = radtodeg(phi);
            betamat(cont) = radtodeg(delta);
            mapRx(cont) = coordx(cont) + sensor_val(5) * cos(phi-pi/2);
            mapRy(cont) = coordy(cont) + sensor_val(5) * sin(phi-pi/2);
            mapLx(cont) = coordx(cont) + sensor_val(4) * cos(phi+pi/2);
            mapLy(cont) = coordy(cont) + sensor_val(4) * sin(phi+pi/2);
            
            % Actualiza el gráfico.
            axes(handles.graf_arana)
            p1 = plot(mapRx,mapRy,'o');
            set(p1,'Color','red');
            hold on
            p2 = plot(mapLx,mapLy,'o');
            set(p2,'Color','blue');
            p3 = plot(coordx,coordy,'+');
            set(p3,'Color','green');
            title('Recorrido')
            hold off

            axes(handles.graf_phi)
            plot(phimat)
            title('Ángulo Araña')

            axes(handles.graf_beta)
            plot(betamat)
            title('Ángulo Destino')

            set(handles.Salida_sensores,'String', ...
                num2cell(reshape(sensor_val(1:5),5,1)))
            txtcix=num2str(coordx(end));
            txtciy=num2str(coordy(end));
            txtphi=num2str(phimat(end));
            txtbet=num2str(betamat(end));
            info = {txtphi;'' ; txtbet;'' ; txtcix;'' ; txtciy;};
            set(handles.inst_info,'String',info)

            axes(handles.graf_polar)
            hex_m = sqrt(coordx.^2 + coordy.^2);
            hex_p = atan2(coordy,coordx);

            ang = 0 : .01 : 2 * pi;
            lenghex = size(hex_m);   
            f_ang = hex_m(1,lenghex(2)) .* ones(size(ang));
            h3a = polar(hex_p,hex_m);
            set(h3a,'color','b','linewidth',2)
            hold on
            h3b = polar(deg2rad(phimat), hex_m);
            set(h3b,'color','r','linewidth',2)
            h3c = polar(ang, f_ang);
            set(h3c,'color','k','linewidth',2,'LineStyle','--')
            hold off
            drawnow
            % pause(0.5);
        catch
            % disp(e);
            disp('No se pudo sincronizar con el mensaje.');
            disp('##################################################');
            flushinput(s);
        end
    end
    
    % Obtiene el valor del checkbox
    checkbox1Value = get(handles.checkbox1, 'value');
    if ~checkbox1Value
        set(handles.text11,'String', 'Ejecutandosé');
    else
        set(handles.text11,'String', 'Detenido');
    end
end
% Elimina de la memoria y del espacio de trabajo el objeto de puerto serie
% 's' con el fin de dejar libre dicho puerto.
fclose(s);
delete(s)
clear s

function cix_Callback(hObject, eventdata, handles)
% hObject    handle to cix (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of cix as text
%        str2double(get(hObject,'String')) returns contents of cix as a double


% --- Executes during object creation, after setting all properties.
function cix_CreateFcn(hObject, eventdata, handles)
% hObject    handle to cix (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function ciy_Callback(hObject, eventdata, handles)
% hObject    handle to ciy (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ciy as text
%        str2double(get(hObject,'String')) returns contents of ciy as a double


% --- Executes during object creation, after setting all properties.
function ciy_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ciy (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes on button press in checkbox1.
function checkbox1_Callback(hObject, eventdata, handles)
% hObject    handle to checkbox1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of checkbox1