//MyEHealthFrameManager
//encapsula el proceso de manipulacion de las tramas del protocolo pactado 
#ifndef MyEHealthFrameManager_H
#define MyEHealthFrameManager_H
#include "Arduino.h"

#define MAX_TEMPERATURE_DECIMAL_PLACES 2
#define MAX_TEMPERATURE_LENGTH 5 
#define MAX_WEIGHT_DECIMAL_PLACES 2
#define MAX_WEIGHT_LENGTH 5
#define MAX_BPM_LENGTH 3
#define MAX_SPO2_LENGTH 3
#define MAX_TIMEID_LENGTH 9
#define MAX_DATATYPE_LENGTH 3
#define MAX_PROTOCOLID_LENGTH 1
#define MAX_SENDERID_LENGTH 3
#define PROTOCOLID "Z"
#define SENDERID "EFC"
#define TEMPERATUREID "T"
#define WEIGHTID "W"
#define BPMSPO2ID "X"
#define T_PACKAGE_MAX_LENGTH 28 //TODO ES CAMPO SE PUEDE CALCULAR ES REDUNDANTE
#define T_PAYLOAD_MAX_LENGTH 12 //TODO ES CAMPO SE PUEDE CALCULAR ES REDUNDANTE
#define X_PACKAGE_MAX_LENGTH 31 //TODO ES CAMPO SE PUEDE CALCULAR ES REDUNDANTE
#define X_PAYLOAD_MAX_LENGTH 15 //TODO ES CAMPO SE PUEDE CALCULAR ES REDUNDANTE
#define W_PACKAGE_MAX_LENGTH 32 //TODO ES CAMPO SE PUEDE CALCULAR ES REDUNDANTE
#define W_PAYLOAD_MAX_LENGTH 16 //TODO ES CAMPO SE PUEDE CALCULAR ES REDUNDANTE
#define TEMPERATURE_LABEL "GRADOS"
#define TL_MAX_LENGTH 6
#define WEIGHT_LABEL "KILOGRAMOS"
#define WL_MAX_LENGTH 10
#define BPM_LABEL "BPM"
#define BL_MAX_LENGTH 3
#define SPO2_LABEL "SAT"
#define SL_MAX_LENGTH 3 
#define FRAME_END_DELIMITER '\n'

//Z WTX EFC 000012301 129 BPM 093 SAT 36.07 GRADOS 70.49 KILOGRAMOS

class MyEHealthFrameManager {
 public:
  MyEHealthFrameManager();
  void setTMeasurement(float);//fija la temperatura
  void setWMeasurement(float);//fija el peso
  void setXMeasurement(int,int);//fija bpm-spo2
  void setDataType(String);////fija el tipo de dato puede ser mas de uno como maximo 3 tipos
  void frameBuilder(String *);//constructor de tramas
  void getTemperature(float *);//recupera la temperatura 
  void getWeight(float *);//recupera la masa
  void getBPM(int *);//recupera bpm
  void getSPO2(int *);//recupera spo2
  void getTimeId(String *);//recupera el timer id
  void getSenderId(String *);//recupera el sender id
  void getDataTypeId(String *);//recupera el tipo dato
  void getProtocolId(String *);//recupera el protocol id
  void enableEndDelimiter(byte);//activa el caracter final de trama
  void readFrame(String frame);//desencapsulado de la trama
private:
  String _temperature;
  String _weight;
  String _bpm;
  String _spo2;
  String _bpmSpo2;
  String _timerId;
  String _senderId;
  String _dataTypeId;
  String _protocolId;
  byte _enEndDelimiter; 
  void parserBuilder(String *,char );//recupera la medida de la trama : dato + etiqueta (xx.xx grados)
  void headerBuilder(String *);//constructor de la header
  void payloadBuilder(String *);//constructor del payload
  void measurementBuilder(String *, char);//constructor del measurement
  void validate(String *,String,byte);//rellena de ceros hasta completar el tamaño maximo
  void _getTimeId(String *);//recupera el timer id
};

#endif
