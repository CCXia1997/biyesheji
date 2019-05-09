package com.ccxia.gui;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Enumeration;

import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.UnsupportedCommOperationException;

public class SerialTool {
	
	//扫描获取可用的串口
	public static final ArrayList<String> uartPortUseAblefind()
	{
		//获取当前所有的可用串口
		Enumeration<CommPortIdentifier> portList=CommPortIdentifier.getPortIdentifiers();
		ArrayList<String> portNameList=new ArrayList();
		//添加并返回ArrayList
		while(portList.hasMoreElements())
		{
			String portName=portList.nextElement().getName();
			portNameList.add(portName);
		}
		return portNameList;
	}
	
	//打开串口
	public static final SerialPort openPort(String portName,int baudrate)
	{
		SerialPort serialPort=null;
		try
		{
			//通过串口名识别串口
			CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portName);
			//打开端口并设置端口名字 serialPort和超时时间 2000ms
	        CommPort commPort=portIdentifier.open(portName,2000);
	        //判断端口是否为串口
	        if(commPort instanceof SerialPort)
	        {
	        	System.out.println("该COM端口是串口！");
	        	serialPort=(SerialPort)commPort;
	        	//设置串口，波特率，8位数据位，1位停止位，无奇偶校验
	        	serialPort.setSerialPortParams(baudrate, SerialPort.DATABITS_8,SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
	        	System.out.println("串口参数设置已完成，波特率为"+baudrate+",数据位8bits,停止位1位,无奇偶校验");
	        }
	        else
	        {
	        	System.out.println("该com端口不是串口,请检查设备!");
	        }
		}
		catch (NoSuchPortException e) 
        {
            e.printStackTrace();
        } 
        catch (PortInUseException e) 
        {
            e.printStackTrace();
        } 
        catch (UnsupportedCommOperationException e)
        {
            e.printStackTrace();
        }
		return serialPort;
	}
	
	//关闭串口
	public static void closePort(SerialPort serialPort)
	{
		if(serialPort!=null)
		{
			serialPort.close();
			serialPort=null;
		}
	}
	
	//上位机往串口发送数据,发送数据为气压设定值对应的电压值（0~255）
	public static void sendToPort(SerialPort serialPort,int settings)
	{
		OutputStream out=null;
		try
		{
			out=serialPort.getOutputStream();
			out.write(settings);
			out.flush();
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
		finally
		{
			if(out!=null)
			{
				try
				{
					out.close();
					out=null;
					System.out.println("数据已发送完毕!");
				}
				catch(IOException e)
				{
					e.printStackTrace();
				}
			}
		}
	}
	
	//上位机从串口接收数据
	public static int readFromPort(SerialPort serialPort)
	{
		int receiveData=0;
		try {
			InputStream in=serialPort.getInputStream();
			int bufferLength=in.available();
		    receiveData=in.read();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return receiveData;
	}
	
	
	
	
	
	
	
	
	
	

}
