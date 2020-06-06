#include "MyEHealthFrameManager.h"

MyEHealthFrameManager::MyEHealthFrameManager(){
   validate(&_protocolId,String(PROTOCOLID),MAX_PROTOCOLID_LENGTH);//validamos el campo protocol id
   validate(&_senderId,String(SENDERID),MAX_SENDERID_LENGTH);//validamos el campo protocol id
}

void MyEHealthFrameManager::validate(String *valid,String invalid,byte len){//agrega "0" hasta completar el tamaño len
  byte x = len - (byte)invalid.length();
  for ( byte y = 0; y < x; y++){
    invalid = "0" + invalid;
  }
  *valid = invalid;
}

void MyEHealthFrameManager::_getTimeId(String *data){//recupera el timer id
  String x = String(millis());
  validate(data,x,MAX_TIMEID_LENGTH);
}  

void MyEHealthFrameManager::setTMeasurement(float temperature){//fija la temperatura
  String x;
  _temperature = "";
  x = String(temperature,MAX_TEMPERATURE_DECIMAL_PLACES);//dato no valida
  validate(&_temperature,x,MAX_TEMPERATURE_LENGTH);//validamos el dato
  _temperature = _temperature + " "+String(TEMPERATURE_LABEL);//fijamos el dato
}

void MyEHealthFrameManager::setWMeasurement(float weight){//fija la masa
  String x;
  _weight = "";
  x = String(weight,MAX_WEIGHT_DECIMAL_PLACES);//dato no valida
  validate(&_weight,x,MAX_WEIGHT_LENGTH);//validamos el dato
  _weight = _weight + " " + String(WEIGHT_LABEL);//fijamos el dato
}

void MyEHealthFrameManager::setXMeasurement(int bpm,int spo2){//fija bpm y spo2
  String x,y;
  _bpmSpo2 = "";
  x = String(bpm);
  y = String(spo2);
  validate(&_bpm,x,MAX_BPM_LENGTH);//validamos el dato bpm
  validate(&_spo2,y,MAX_SPO2_LENGTH);//validamos el dato spo2
  _bpmSpo2 = _bpm + " " + String(BPM_LABEL) + " " + _spo2 + " " + String(SPO2_LABEL); //validamos el bpmSpo2
}

void MyEHealthFrameManager::setDataType(String dataTypeId){//fijamos el data Type
  _dataTypeId = "";
  validate(&_dataTypeId,dataTypeId,MAX_DATATYPE_LENGTH); //validamos el campo data type 
}


void MyEHealthFrameManager::headerBuilder(String *header){//creamos al cabecera
  String h;
  h = _protocolId + _dataTypeId;//agregamos los campos protocol id y data type
  *header = h;//agregamos la cabecera al paquete
}

void MyEHealthFrameManager::measurementBuilder(String *measurement,char dataTypeId){//creamos el campo measurement de la trama 
  if (dataTypeId == 'T'){//para la temperatura
    *measurement = *measurement + _temperature; 
  }
  else if (dataTypeId == 'X'){//para bpm-spo2
    *measurement = *measurement + _bpmSpo2;
  }
  else if (dataTypeId  == 'W'){//para la masa
    *measurement = *measurement + _weight; 
  }
}

void MyEHealthFrameManager::payloadBuilder(String *payload){//constructor del payload
  String mt,pld;
  byte counter =  0;//numero maximo de medidas
  char pointer;//medida actaul
  while  (counter < MAX_DATATYPE_LENGTH ){
    pointer = _dataTypeId.charAt( counter ); 
    counter += 1;
    if (pointer == '0'){//no es un tipo
      continue;
    }
    else{//es un tipo de dato
      measurementBuilder(&mt,pointer);//agregamos la medida 
    }
  }
  _getTimeId(&_timerId);//recuperamos el timer Id
  pld =  _senderId + _timerId + mt;//creamos el payload
  *payload = pld;//copiamos el payload
}

void MyEHealthFrameManager::enableEndDelimiter(byte onOff){//habilitamos/deshabilitamos la opcion trama con caracter de final de trama
  _enEndDelimiter = onOff;
}

void MyEHealthFrameManager::frameBuilder(String *frame){//creamos la trama
  String header,payload;
  headerBuilder(&header);//creamos la cabecera
  payloadBuilder(&payload);//creamos el payload
  *frame = header + payload;//creamos la trama
  if (_enEndDelimiter){//agregamos el caracter final de trama
    *frame += String(FRAME_END_DELIMITER); 
  }
}

void MyEHealthFrameManager::parserBuilder(String *payload,char dataTypeId){//desencapsula los campos measurement de la trama, todos llegan la etiquea
  if (dataTypeId == 'T'){//para la temperatura
    _temperature = payload->substring(0,T_PAYLOAD_MAX_LENGTH);//fijamos la temperatura
    *payload =  payload->substring(T_PAYLOAD_MAX_LENGTH);//avanzamos en el proceso
  }
  else if (dataTypeId == 'X'){//para bpm-spo2
    _bpmSpo2 = payload->substring(0,X_PAYLOAD_MAX_LENGTH);//fijamos bpm-spo2
    *payload =  payload->substring(X_PAYLOAD_MAX_LENGTH);//avanzamos en el proceso
  }
  else if (dataTypeId  == 'W'){//para la masa
    _weight = payload->substring(0,W_PAYLOAD_MAX_LENGTH);//fijamos la masa
    *payload =  payload->substring(W_PAYLOAD_MAX_LENGTH);//avanzamos en el proceso
  }
}

void MyEHealthFrameManager::readFrame(String frame){//desencapsulado de la trama
 String pl,mt; 
 char pointer;//medida actaul
 byte counter; 
 _protocolId = frame.substring(0,MAX_PROTOCOLID_LENGTH);//recuperamos el protocol id
 _dataTypeId = frame.substring(MAX_PROTOCOLID_LENGTH,MAX_PROTOCOLID_LENGTH+MAX_DATATYPE_LENGTH);//recuperamos el data type
 _senderId = frame.substring(MAX_PROTOCOLID_LENGTH+MAX_DATATYPE_LENGTH,MAX_PROTOCOLID_LENGTH+MAX_DATATYPE_LENGTH+MAX_SENDERID_LENGTH);//recuperamos sender id
 _timerId = frame.substring(MAX_PROTOCOLID_LENGTH+MAX_DATATYPE_LENGTH+MAX_SENDERID_LENGTH,MAX_PROTOCOLID_LENGTH+MAX_DATATYPE_LENGTH+MAX_SENDERID_LENGTH+MAX_TIMEID_LENGTH);//recuperamos time id
 pl = frame.substring(MAX_PROTOCOLID_LENGTH+MAX_DATATYPE_LENGTH+MAX_SENDERID_LENGTH+MAX_TIMEID_LENGTH);//recuperamos el payload
 counter =  0;//numero maximo de medidas
 while  (counter < MAX_DATATYPE_LENGTH ){//mientra no se alcance el numero maximo de tipos de datos 
    pointer = _dataTypeId.charAt( counter ); //tipo de dato actual
    counter += 1;//pasos al siguiente
    if (pointer == '0'){//no es un tipo
      continue;//siguiente iteracion
    }
    else{//es un tipo de dato
      parserBuilder(&pl,pointer);//desencapsulamos el tipo de dato actual
    }
  }
}

void MyEHealthFrameManager::getTemperature(float *temperature){//recuperamos la temperatura 
  *temperature = _temperature.substring(0,MAX_TEMPERATURE_LENGTH).toFloat();
}

void MyEHealthFrameManager::getWeight(float *weight){//recuperamos la masa
  *weight = _weight.substring(0,MAX_TEMPERATURE_LENGTH).toFloat();
}

void MyEHealthFrameManager::getBPM(int *BPM){//recuperamos  bpm
*BPM = _bpmSpo2.substring(0,MAX_BPM_LENGTH).toInt();
}

void MyEHealthFrameManager::getSPO2(int *spo2){//recuperamos spo2 
 *spo2 = _bpmSpo2.substring(MAX_BPM_LENGTH + BL_MAX_LENGTH + 2,MAX_BPM_LENGTH + BL_MAX_LENGTH + 2 + MAX_SPO2_LENGTH).toFloat();
}

void MyEHealthFrameManager::getTimeId(String *timeId){//recuperamos el timer id
  *timeId = _timerId;
}

void MyEHealthFrameManager::getSenderId(String *senderId){//recuperamos el sender id
  *senderId = _senderId;
}

void MyEHealthFrameManager::getDataTypeId(String *dataTypeId){//recuperamos el data type
  *dataTypeId = _dataTypeId;
}

void MyEHealthFrameManager::getProtocolId(String *protocolId){//recuperamos el protocol id
  *protocolId = _protocolId;
}



