#include <eHealth.h>
#include <MyEHealthFrameManager.h>
#include <MPXbee.h>
#include <Monitor.h>
MPXbee mpxbee;
Monitor monitor;
MyEHealthFrameManager eFrame;//instancia de la clase MyEHealthFrameManager para desencapsular la trama
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
String frame,timeId,protocolId,dataType,senderId;//campos de trama
byte tFlag = 0,wFlag = 0,xFlag = 0;//flag que alertan de la recepcion de datos : temperatura,masa y bpm-spo2
float temperature,weight;//temperatura y peso
int bpm,spo2;//bpm y spo2
int alertT,alertS,alertB1,alertB2;//contadores para el total de incidencias
char dt;//tipo de dato actual
mpxbee.readData(0,&frame);//lectura de la trama, el primer parametro es 0 por lo tanto se lee la trama hasta el caracter final de trama
eFrame.readFrame(frame);//desencapsulado de la trama
eFrame.getTimeId(&timeId);//recupera el timer id
eFrame.getProtocolId(&protocolId);//recupera el protocol id
eFrame.getDataTypeId(&dataType);//recupera el data type id
eFrame.getSenderId(&senderId);//recupera el sender id
monitor.writeMessage("Protocol : " + protocolId + " Data Type :" + dataType + " Sender : " + senderId + " Time : " + timeId+" ");

for ( byte x = 0 ; x < MAX_DATATYPE_LENGTH ; x++){//por cada uno de los tipos de datos de la trama
  dt = dataType.charAt(x);//seleccionar el tipo actual
  if (dt != '0')
    if (dt  == 'T'){//temperatura
      eFrame.getTemperature(&temperature);//recupera la temperatura
      tFlag=1;//sube el flag
      monitor.writeMessage("Temperatura : " + String(temperature,MAX_TEMPERATURE_DECIMAL_PLACES)+" ");
    }
    else if (dt  == 'W'){//masa
      eFrame.getWeight(&weight);//recupera la masa
      wFlag = 1;//sube el flag
      monitor.writeMessage("Peso : " + String(weight,MAX_WEIGHT_DECIMAL_PLACES)+" ");
    }
    else if (dt  == 'X'){//bpm-spo2
      eFrame.getBPM(&bpm);//recupera el bpm
      eFrame.getSPO2(&spo2);//recupera la spo2
      xFlag = 1;//sube el flag
      monitor.writeMessage("BPM : " + String(bpm) + " SPO2 :" + String(spo2));
    }
}
if ( tFlag || xFlag || wFlag)
  monitor.writeMessage("\r\n");

if (tFlag){
  if (temperature > 38){//alerta temperatura
    monitor.writeMessage("ATENCION : LIMITE MÁXIMO DE TEMPERATURA SUPERADO\r\n");
    alertT++;//actualizar el numero de alertas
  }
}
if (xFlag){
   if (spo2< 95){//alerta spo2
    monitor.writeMessage("ATENCION : LIMITE MINIMO DE SPO NO ALCANZADO\r\n");
    alertS++;//actualizacion el numero de alertas
   }
   if (bpm < 35){//alerta bpm minimo
    monitor.writeMessage("ATENCION : LIMITE MINIMO DE BPM NO ALCANZADO\r\n");
    alertB1++;
   }
   else if (bpm > 110){//alerta bpm maximo
    monitor.writeMessage("ATENCION : LIMITE MAXIMO DE BPM SUPERADO\r\n");
    alertB2++;
   } 
}
tFlag  = xFlag = wFlag = 0;//bajamos los flag
}
