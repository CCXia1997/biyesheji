function varargout = bihuan(varargin)
% BIHUAN MATLAB code for bihuan.fig
%      BIHUAN, by itself, creates a new BIHUAN or raises the existing
%      singleton*.
%
%      H = BIHUAN returns the handle to a new BIHUAN or the handle to
%      the existing singleton*.
%
%      BIHUAN('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in BIHUAN.M with the given input arguments.
%
%      BIHUAN('Property','Value',...) creates a new BIHUAN or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before bihuan_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to bihuan_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help bihuan

% Last Modified by GUIDE v2.5 24-Apr-2019 15:03:21

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @bihuan_OpeningFcn, ...
                   'gui_OutputFcn',  @bihuan_OutputFcn, ...
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


% --- Executes just before bihuan is made visible.
function bihuan_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to bihuan (see VARARGIN)
global COM;
global rate;
COM='COM6';
rate=9600;
set(handles.ppcom,'Value',6);
set(handles.ppbaudrates,'Value',2);
set(handles.pbcloseserial,'Enable','off');
set(handles.axes1,'XLim',[0 10],'YLim',[0 255]);


% Choose default command line output for bihuan
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes bihuan wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = bihuan_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on selection change in ppcom.
function ppcom_Callback(hObject, eventdata, handles)
% hObject    handle to ppcom (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns ppcom contents as cell array
%        contents{get(hObject,'Value')} returns selected item from ppcom
global COM;
val=get(hObject,'Value');
switch val
    case 1
        COM='COM1';
    case 2
        COM='COM2';
    case 3
        COM='COM3';
    case 4
        COM='COM4';
    case 5
        COM='COM5';
    case 6
        COM='COM6';
    case 7
        COM='COM7';
end

% --- Executes during object creation, after setting all properties.
function ppcom_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ppcom (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pbopenserial.
function pbopenserial_Callback(hObject, eventdata, handles)
% hObject    handle to pbopenserial (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
clc
instrreset
global s;
global COM;
global rate;

s=serial(COM);
set(s,'BaudRate',rate,'DataBits',8,'Parity','none','StopBits',1);
set(s,'inputBufferSize',4096);
set(handles.pbopenserial,'Enable','off');
set(handles.pbcloseserial,'Enable','on');
set(s,'BytesAvailableFcnMode','byte');
set(s,'BytesAvailableFcnCount',300);
s.BytesAvailableFcn={@ReceiveCallBack,handles};
fopen(s);
fprintf('open com\n');

% --- Executes on selection change in ppbaudrates.
function ppbaudrates_Callback(hObject, eventdata, handles)
% hObject    handle to ppbaudrates (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns ppbaudrates contents as cell array
%        contents{get(hObject,'Value')} returns selected item from ppbaudrates
global rate;
val=get(hObject,'Value');
switch val
    case 1
        rate=9600;
    case 2
        rate=19200;
    case 3
        rate=38400;
    case 4
        rate=115200;
end

% --- Executes during object creation, after setting all properties.
function ppbaudrates_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ppbaudrates (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pbcloseserial.
function pbcloseserial_Callback(hObject, eventdata, handles)
% hObject    handle to pbcloseserial (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global s;
fclose(s);
delete(s);
set(handles.pbopenserial,'Enable','on');
set(handles.pbcloseserial,'Enable','off');
fprintf('close com\n');


function editqiya_Callback(hObject, eventdata, handles)
% hObject    handle to editqiya (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editqiya as text
%        str2double(get(hObject,'String')) returns contents of editqiya as a double


% --- Executes during object creation, after setting all properties.
function editqiya_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editqiya (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in pbforstart.
function pbforstart_Callback(hObject, eventdata, handles)
% hObject    handle to pbforstart (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global s;
global set_point;
set_point=str2double(get(handles.editqiya,'String'));
for i=1:50
    fwrite(s,set_point);
    pause(0.2);
end


% --- Executes on selection change in editR.
function editR_Callback(hObject, eventdata, handles)
% hObject    handle to editR (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns editR contents as cell array
%        contents{get(hObject,'Value')} returns selected item from editR


% --- Executes during object creation, after setting all properties.
function editR_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editR (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes during object creation, after setting all properties.
function figure1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% --- Executes on button press in pbfordisplay.
function pbfordisplay_Callback(hObject, eventdata, handles)
% hObject    handle to pbfordisplay (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
global s;
global receive;
global time;
global qiya;
receive=num2str(fread(s,50));
set(handles.editR,'String',receive);
time=0.2:0.2:10;
qiya=str2num(receive);
plot(handles.axes1,time,qiya,'b-','LineWidth',1.5);
