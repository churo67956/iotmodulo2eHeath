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
//Send data
String frame;
eFrame.setDataType("TX");//fijamos el tipo de dato de la trama como se puede observar se envia mas de un dato
eFrame.setTMeasurement(eHealth.getTemperature());//fijamos la temperatura
eFrame.setXMeasurement(eHealth.getBPM(),eHealth.getOxygenSaturation());//fijamos bpm y spo2
eFrame.frameBuilder(&frame);//construimos la trama
mpxbee.writeData(T_PACKAGE_MAX_LENGTH+X_PAYLOAD_MAX_LENGTH,frame);//enviamos la trama
monitor.writeMessage(frame);
monitor.writeMessage("\r\n");
delay( 3000 );
}
