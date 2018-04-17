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
clc;
delete(instrfindall);

cf=[str2num(get(handles.cfx,'String')),str2num(get(handles.cfy,'String'))];       %coordenadas de destino
ci=[str2num(get(handles.cix,'String')),str2num(get(handles.ciy,'String'))];       %coordenadas iniciales de la araña
phi=pi/2;                                                                           %angulo inicial de la araña
A=8;                                                                                %cuanto avanza la araña por cada forward (cm)
alphal=degtorad(90/7);                                                           %angulo de giro de la araña izquierda
alphar=degtorad(90/7);                                                           %angulo de giro de la araña derecha
error=10;                                                                         %umbral de error aceptable    (cm)


                
fzd = readfis('Fuzzy_Logic_Design_2018.fis');

comPort = '/dev/tty.usbmodem1431';

s = serial(comPort);
set(s,'DataBits',8);
set(s,'StopBits',1);
set(s,'BaudRate',115200);
set(s,'Parity','none');
fopen(s);
i=1;
checkbox1Value = get(handles.checkbox1, 'value');
while (((abs(cf(1)-ci(1)) > error) || (abs(cf(2)-ci(2)) > error)) && checkbox1Value == 0 ) 
    checkbox1Value = get(handles.checkbox1, 'value');
    a=num2str(checkbox1Value);
   
    set(handles.text11,'String',a)
    
    %angulo entre el orgien y el destino
    delta=atan((cf(2)-ci(2))/(cf(1)-ci(1)));

    %II cuadrante
    if (cf(1)-ci(1)<0 && cf(2)-ci(2)>0)                 
        gama=delta+pi;
    else
        %I cuadrante
        if (cf(1)-ci(1)>0 && cf(2)-ci(2)>0)
            gama=delta;
        else
            %III cuadrante
            if (cf(1)-ci(1)<0 && cf(2)-ci(2)<0)
                gama=delta+pi;
            else
                %IV cuadrante
                if (cf(1)-ci(1)>0 && cf(2)-ci(2)<0)
                    gama=delta;
                else
                    gama=0;
                end
            end
        end
    end

    % Obtiene las lecturas del puerto serie
    sensorLect = fgets(s);
    disp(sensorLect);
    % Verificamos si los primeros 3 caracteres son los necesarios para
    % ejecutar la lògica difusa, en este caso, MSG.
    if (extractBefore(sensorLect,4) == 'MSG')
        dirSL = strsplit(sensorLect,':');
        for c = 1:length(dirSL)
            if str2double(dirSL{1,c}) > 100
                dirSL{1,c} = '100';
            end
        end
        sensorFU = str2double(dirSL{1,2});
        sensorRU = str2double(dirSL{1,3});
        sensorLU = str2double(dirSL{1,4});
        sensorR = str2double(dirSL{1,5});
        sensorL = str2double(dirSL{1,6});
        slope_y = str2double(dirSL{1,7});

        betarad=radtodeg(gama);
        resultFZD = evalfis([sensorFU,sensorLU,sensorRU,sensorL,sensorR,betarad,slope_y],fzd);

        if (resultFZD >= 0 && resultFZD <1) 
            fprintf(s, '%c', 'L');
            set(handles.Fuzzy_out,'String','Izquierda')
            phi=phi+alphal;
        else
            if (resultFZD >=1 && resultFZD < 2)
                fprintf(s, '%c', 'F');
                set(handles.Fuzzy_out,'String','Adelante')
                ci(1)=ci(1)+A*cos(phi);
                ci(2)=ci(2)+A*sin(phi);
                coordx(i)=ci(1);
                coordy(i)=ci(2);
                mapRx(i)=coordx(i)+sensorR*cos(phi-pi/2);
                mapRy(i)=coordy(i)+sensorR*sin(phi-pi/2);
                mapLx(i)=coordx(i)+sensorL*cos(phi+pi/2);
                mapLy(i)=coordy(i)+sensorL*sin(phi+pi/2);
                axes(handles.graf_arana)
                p1=plot(mapRx,mapRy,'o');
                set(p1,'Color','red')
                hold on
                p2=plot(mapLx,mapLy,'o');
                set(p2,'Color','blue')
                p3=plot(ci(1),ci(2));
                set(p3,'Color','green')
                title('Mapa')
            else
                if (resultFZD >=2 && resultFZD < 3)
                    fprintf(s, '%c', 'R');
                    set(handles.Fuzzy_out,'String','Derecha')
                    phi=phi-alphar;
                else
                    fprintf(s, '%c', 'H');
                    set(handles.Fuzzy_out,'String','Wave')
                end
            end
        end
        phimat(i)=radtodeg(phi);
        betamat(i)=betarad;
        coordx(i)=ci(1);
        coordy(i)=ci(2);
        sensFU=num2str(sensorFU);
        sensRU=num2str(sensorRU);
        sensR=num2str(sensorR);
        sensLU=num2str(sensorLU);
        sensL=num2str(sensorL);
    else
        phimat(i)=0;
        betamat(i)=radtodeg(gama);
        coordx(i)=0;
        coordy(i)=0;
        sensFU='Default';
        sensRU='Default';
        sensR='Default';
        sensLU='Default';
        sensL='Default';
    end
    
    
    axes(handles.graf_arana)
    plot(coordx,coordy)
    title('Recorrido')
    
    axes(handles.graf_phi)
    plot(phimat)
    title('Ángulo Araña')
    
    axes(handles.graf_beta)
    plot(betamat)
    title('Ángulo Destino')
    
    
    sensALL = {sensFU; '' ; sensRU;'' ; sensLU;'' ; sensR;'' ; sensL};
    set(handles.Salida_sensores,'String',sensALL)
    txtcix=num2str(coordx(i));
    txtciy=num2str(coordy(i));
    txtphi=num2str(phimat(i));
    txtbet=num2str(betamat(i));
    info = {txtphi;'' ; txtbet;'' ; txtcix;'' ; txtciy;};
    set(handles.inst_info,'String',info)
    
    
    axes(handles.polar)
    phimat_r = degtorad(phimat);
    hex_m = sqrt(coordx.^2 + coordy.^2);
    hex_p = atan2(coordy,coordx);

    ang = 0 : .01 : 2 * pi;
    lenghex = size(hex_m);   
    f_ang = hex_m(1,lenghex(2)) .* ones(size(ang));
    h3a = polar(hex_p,hex_m);
    set(h3a,'color','b','linewidth',2)
    hold on
    h3b = polar(phimat_r, hex_m);
    set(h3b,'color','r','linewidth',2)
    h3c = polar(ang, f_ang);
    set(h3c,'color','k','linewidth',2,'LineStyle','--')
    hold off
    

    
    drawnow
    
    i=i+1;
end



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
