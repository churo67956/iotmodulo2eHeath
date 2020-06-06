#include <eHealth.h>
#include <MyEHealthFrameManager.h>
#include <MPXbee.h>
#include <Monitor.h>
MPXbee mpxbee;
Monitor monitor;
MyEHealthFrameManager eFrame;
byte mpxbeeChannel;
int mpxbeeOpPAN;
unsigned int mpxbeeSrcAddress;
unsigned int mpxbeeDestAddressH;
unsigned int mpxbeeDestAddressL;
String monitorMessage;

void setup(){
//Initialize Arduino-PC serial communication
monitor.Init();
//Initialize XBee module
monitor.writeMessage( "Initializing module..." );
monitor.writeMessage( "\r\n" );
mpxbee.Init( MPXBEE_SERIALPORT_1 );
//Read XBee parameters
mpxbee.getChannel( &mpxbeeChannel );
mpxbee.getOperationPAN( &mpxbeeOpPAN );
mpxbee.getSrcAddress( &mpxbeeSrcAddress );
mpxbee.getDestAddress( &mpxbeeDestAddressH, &mpxbeeDestAddressL );
//Print XBee parameters
monitor.writeMessage( "Channel: " );
monitorMessage = String( mpxbeeChannel, HEX );
monitor.writeMessage( monitorMessage );
monitor.writeMessage( "\r\n" );
monitor.writeMessage( "Operation PAN: " );
monitorMessage = String( mpxbeeOpPAN, HEX );
monitor.writeMessage( monitorMessage );
monitor.writeMessage( "\r\n" );
monitor.writeMessage( "Source address: " );
monitorMessage = String( mpxbeeSrcAddress, HEX );
monitor.writeMessage( monitorMessage );
monitor.writeMessage( "\r\n" );
monitor.writeMessage( "Destination address: " );
monitorMessage = String( mpxbeeDestAddressH, HEX );
monitor.writeMessage( monitorMessage );
monitor.writeMessage( " " );
monitorMessage = String( mpxbeeDestAddressL, HEX );
monitor.writeMessage( monitorMessage );
monitor.writeMessage( "\r\n" );
delay( 500 );
}
void loop(){
String frame,timeId,protocolId,dataType,senderId;//campos de la trama 
int bpm,spo2;
mpxbee.readData(X_PACKAGE_MAX_LENGTH,&frame);//leemos la trama
eFrame.readFrame(frame);//desencapsulamos la trama
eFrame.getBPM(&bpm);//recuperamos el bpm
eFrame.getSPO2(&spo2);//recuperamos el spo2
eFrame.getTimeId(&timeId);//recuperamos el timer id
eFrame.getProtocolId(&protocolId);//recuperamos el protocol id
eFrame.getDataTypeId(&dataType);//recuperamos el data type
eFrame.getSenderId(&senderId);//recuperamos el sender id
monitor.writeMessage("Protocol : " + protocolId + "\r\nData Type :" + dataType + "\r\nSender : " + senderId + "\r\nTime : " + timeId +"\r\nBPM: " + bpm +"\r\nSPO: " + spo2);//escribimos por el puerto serie la trama desencapsulada
monitor.writeMessage( "\r\n" );
delay( 1000 );
}
