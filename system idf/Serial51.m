8function Serial51()
    delete(instrfindall) 
    global s;
    s=serial('COM6');
    set(s,'BaudRate',9600,'DataBits',8,'Parity','none','StopBits',1);
    set(s,'inputBufferSize',4096);
    set(s,'BytesAvailableFcnMode','byte');
    set(s,'BytesAvailableFcnCount',200);
    s.BytesAvailableFcn =@ReceiveCallback;
    fopen(s);
    for i=1:40
        fwrite(s,134);
        pause(0.2);
    end
    for i=1:40
        fwrite(s,193);
        pause(0.2);
    end
    for i=1:40
        fwrite(s,134);
        pause(0.2);
    end
    %fclose(s);
    %delete(s);
    %clear s;
end
function ReceiveCallback(obj,event)
    global s;
    global receive;
    receive=fread(s,10)
end

    

