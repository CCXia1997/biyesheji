package com.ccxia.gui;

import java.awt.Button;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Label;
import java.awt.TextArea;
import java.awt.TextField;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import javax.swing.ImageIcon;
import javax.swing.JFrame;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartFrame;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

import gnu.io.SerialPort;


public class SerialGui {
	public SerialPort s=null;
	//添加多行文本框显示接收数据
	public TextArea ta_receive=new TextArea(20,100);
	public void login()
	{
		//设置icon图标
	    ImageIcon icon = new ImageIcon("src/resources/bookshelf.png");
		Frame f=new Frame();
		f.setTitle("压力闭环控制系统");
		f.setSize(680,500);
		f.setLocation(280,130);
		f.setLayout(null);
		f.setIconImage(icon.getImage());
		//设置界面大标题
		Label label_title=new Label("压力闭环控制系统byCCXia");
		label_title.setFont(new Font("微软雅黑",Font.BOLD,40));
		label_title.setForeground(Color.RED);
		label_title.setSize(600,50);
		label_title.setLocation(100,50);
		f.add(label_title);
		//设置串口号，波特率，设定值文字，接收区的显示
		Label label_serial=new Label("串口号");
		Label label_baudrate=new Label("波特率");
		Label label_settings=new Label("设定值");
		Label label_receive=new Label("接收区");
		label_serial.setFont(new Font("微软雅黑",Font.BOLD,30));
		label_baudrate.setFont(new Font("微软雅黑",Font.BOLD,30));
		label_settings.setFont(new Font("微软雅黑",Font.BOLD,30));
		label_receive.setFont(new Font("微软雅黑",Font.BOLD,20));
		label_serial.setSize(100,30);
		label_serial.setLocation(100,120);
		label_baudrate.setSize(100,30);
		label_baudrate.setLocation(100,180);
		label_settings.setSize(100,30);
		label_settings.setLocation(100,240);
		label_receive.setSize(80,20);
		label_receive.setLocation(400,110);
		
		f.add(label_serial);
		f.add(label_baudrate);
		f.add(label_settings);
		f.add(label_receive);
		
		//设置串口号，波特率，设定值的文本输入界面，弹出式菜单过于麻烦不做了
		final TextField tf_serial=new TextField(20);
		final TextField tf_baudrate=new TextField(20);
		final TextField tf_settings=new TextField(20);
		tf_serial.setSize(100,30);
		tf_serial.setLocation(200,122);
		tf_baudrate.setSize(100,30);
		tf_baudrate.setLocation(200,182);
		tf_settings.setSize(100,30);
		tf_settings.setLocation(200,242);
		
		f.add(tf_serial);
		f.add(tf_baudrate);
		f.add(tf_settings);
		
		//设置接收区
		ta_receive.setSize(200,330);
		ta_receive.setLocation(350,140);
		f.add(ta_receive);
		
		
		//设置打开串口，关闭串口，控制开始按钮
		Button bu_open=new Button("打开串口");
		Button bu_close=new Button("关闭串口");
		Button bu_start=new Button("开始控制");
		bu_open.setSize(200,50);
		bu_open.setLocation(100,300);
		bu_close.setSize(200,50);
		bu_close.setLocation(100,360);
		bu_start.setSize(200,50);
		bu_start.setLocation(100,420);
		
		//设置按钮事件
		bu_open.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e)
			{
				//从文本框中获取对应的串口号和波特率
				String portName=tf_serial.getText().trim();
				int baudrate=Integer.valueOf(tf_baudrate.getText().trim());
				s=SerialTool.openPort(portName,baudrate);
			}
		});
		
		bu_close.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e)
			{
				SerialTool.closePort(s);
			}
		});
		
		bu_start.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e)
			{
				//设置图表区
				//创建一个储存x，y数据的东西
				XYSeries series=new XYSeries("xySeries");
				int receive_data;
				int settings=Integer.valueOf(tf_settings.getText().trim());
				for(int i=0;i<50;i++)
				{
					try {
						SerialTool.sendToPort(s,settings);
						settings++;
						Thread.sleep(200);
					} catch (InterruptedException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					receive_data=SerialTool.readFromPort(s);
					ta_receive.append("时间："+String.format("%.1f",(i+1)*0.2)+"s    "+String.valueOf(receive_data)+"\r\n");
					//给series添加（x，y）数据
					series.add((i+1)*0.2,receive_data);
				}
				//循环结束以后根据数据进行画图
				XYSeriesCollection dataset = new XYSeriesCollection();
				dataset.addSeries(series);
				//设置图表的基本参数
				JFreeChart chart = ChartFactory.createXYLineChart("气压变化曲线图","时间/s","气压值",dataset,PlotOrientation.VERTICAL,false,false,false);
				ChartFrame frame = new ChartFrame("my picture", chart);
				frame.pack();
				frame.setVisible(true);
				frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
			}
		});
		
		f.add(bu_open);
		f.add(bu_close);
		f.add(bu_start);
		
		
		//让窗口关闭
		//用适配器类改进
		f.addWindowListener(new WindowAdapter() {
			@Override
			public void windowClosing(WindowEvent e)
			{
				System.exit(0);
			}
		});
		
		
		
		
		
		
		
		
		
		
		f.setVisible(true);
	}
	
	//绘制图表
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

}
