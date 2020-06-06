#include <eHealth.h>
#include <MyEHealthFrameManager.h>
#include <MPXbee.h>
#include <Monitor.h>
#define MAX_DAYS 3 //numero maximo de dias
#define MAX_DAY_DELAY 241000 //duracion maxima de un dia, un poquito mas para enviar la notificion de las 24:00 
#define MAX_BPM_SPO2_DELAY_TIME 5000//intervalo de tiempo para enviar los datos BPM y SPO2
#define MAX_TEMPERATURA_DELAY_TIME 80000//intervalo de tiempo para enviar de la temperatura
#define MAX_WEIGHT_DELAY_TIME 90000//intervalo de tiempo para enviar de el peso
unsigned long initTime,lastTimeW,lastTimeBS,lastTimeT,currentTime;//variables que llevan el instante del ultimo envio de los parametros
byte wloop,tloop,xloop;//estas variables las empleo para saber si los envios son periodicos, por ejemplo wloop tras el primer envio se pone a 0
byte myCounter;//contador de los dias restantes

MPXbee mpxbee;
Monitor monitor;
MyEHealthFrameManager eFrame;;//instancia de la clase MyEHealthFrameManager para formar las tramas
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
myCounter = 0;//contador de dias a cero
initTime = millis();//tiempo de inicio
lastTimeW = lastTimeBS = lastTimeT = initTime;//todos temporizadores igual al tiempo de inicio
wloop = tloop = xloop = 1;//envio periodicos activado, al menos una vez
monitor.writeMessage("Dia 1 \r\n");
}
void loop(){
//Send data
  String frame,dataType;
  currentTime = millis();
  if ( myCounter < MAX_DAYS){//no han pasado los 3 dias
    if  (currentTime >= initTime + MAX_DAY_DELAY){//cada dia que pasa
      initTime = lastTimeW = lastTimeBS = lastTimeT = currentTime;//reiniciar los temporizadores
      myCounter ++;//incrementar el contador de dias
      monitor.writeMessage("Dia " + String(myCounter + 1) + " \r\n");
    }
    else{
      if (currentTime  >= lastTimeBS  + MAX_BPM_SPO2_DELAY_TIME && xloop){//envio programado de BPM y SPO2
        dataType = "X";//actulizamos el tipo de dato
        lastTimeBS = currentTime;//actualizamos el temporizador corrrespondiente 
        eFrame.setXMeasurement(eHealth.getBPM(),eHealth.getOxygenSaturation());//actualizamos el payload de la trama

      }
      if (currentTime  >= lastTimeT  + MAX_TEMPERATURA_DELAY_TIME && tloop){//envio programado de la temperatura
        dataType += "T";//actulizamos el tipo de dato
        lastTimeT= currentTime;//actualizamos el temporizador corrrespondiente 
        eFrame.setTMeasurement(eHealth.getTemperature());//actualizamos el payload de la trama
      }
      if (currentTime  >= lastTimeW  + MAX_WEIGHT_DELAY_TIME && wloop){//envio programado de la masa
        dataType += "W";//actulizamos el tipo de dato
        wloop = 0;//solo un envia cada dia
        lastTimeW = currentTime;//actualizamos el temporizador corrrespondiente 
        eFrame.setWMeasurement(eHealth.getWeight());//actualizamos el payload de la trama   
      }
      if (dataType.length() > 0){//existe algun tipo de dato para enviar
        eFrame.enableEndDelimiter(1);//introducimos el caracter final de trama para realizar el envio  
        eFrame.setDataType(dataType);//actualizamos el tipo de dato de la trama
        eFrame.frameBuilder(&frame);//creamos la trama
        mpxbee.writeData(frame.length(),frame);//enviamos la trama
        monitor.writeMessage(frame);
        monitorMessage = "";
      }
    }
  }
  else{// cuarto dia
    return;  
  }
}
