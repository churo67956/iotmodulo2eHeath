//MPXBee
//Version: 2.0

#include "Arduino.h"
#include "MPXbee.h"

MPXbee::MPXbee( void )
{
  _serialPort = MPXBEE_SERIALPORT_UNDEF;
  _opChannel = MPXBEE_CHANNEL_UNDEF;
  _opPAN = MPXBEE_PAN_UNDEF;
  _nwkAddress = MPXBEE_NWKADDRESS_UNDEF;
  _destAddressH = MPXBEE_DESTADDRESSH_UNDEF;
  _destAddressL = MPXBEE_DESTADDRESSL_UNDEF;
}

void MPXbee::SerialInit( byte serialPort )
{
  //Initialize serial communication
  switch( serialPort )
  {
    case MPXBEE_SERIALPORT_0:
    Serial.begin( 9600 );
    _serialPort = serialPort;
    break;
    case MPXBEE_SERIALPORT_1:
    Serial1.begin( 9600 );
    _serialPort = serialPort;
    break;
    case MPXBEE_SERIALPORT_2:
    Serial2.begin( 9600 );
    _serialPort = serialPort;
    break;
    case MPXBEE_SERIALPORT_3:
    Serial3.begin( 9600 );
    _serialPort = serialPort;
    break;
    default:
    break;
  }
}

boolean MPXbee::Init( byte serialPort )
{
  //Declarations
  String paramValue = "";
  String paramValue1 = "";
    
  //Initialize serial communication
  switch( serialPort )
  {
    case MPXBEE_SERIALPORT_0:
    Serial.begin( 9600 );
    _serialPort = serialPort;
    break;
    case MPXBEE_SERIALPORT_1:
    Serial1.begin( 9600 );
    _serialPort = serialPort;
    break;
    case MPXBEE_SERIALPORT_2:
    Serial2.begin( 9600 );
    _serialPort = serialPort;
    break;
    case MPXBEE_SERIALPORT_3:
    Serial3.begin( 9600 );
    _serialPort = serialPort;
    break;
    default:
    return false;
    break;
  }

  //Wait network association
  while( !getAssoc() )
  {
    delay( random( 500, 2500 ) );
  }
  //Read operation channel
  while( !getOpChannel( &paramValue ) )
  {
    delay( random( 10, 500 ) );
  }
  _opChannel = ( byte )strtol( paramValue.c_str(), NULL , 16 );
  
  //Read operation PAN
  while( !getOpPAN( &paramValue ) )
  {
    delay( random( 10, 500 ) );
  }
  _opPAN = ( int )strtol( paramValue.c_str(), NULL , 16 );
  
  //Read network address
  while( !getNwkAddress( &paramValue ) )
  {
    delay( random( 10, 500 ) );
  }
  _nwkAddress = ( int )strtol( paramValue.c_str(), NULL , 16 );
  //Read destination address
  while( !getDestinationAddress( &paramValue, &paramValue1 ) )
  {
    delay( random( 10, 500 ) );
  }
  _destAddressH = ( int )strtol( paramValue.c_str(), NULL, 16 );
  _destAddressL = ( int )strtol( paramValue1.c_str(), NULL, 16 );
 
  return true;
}

void MPXbee::getSerialPort( byte *serialPort )
{
  //Read serial port communication with xbee module
  serialPort[ 0 ] = _serialPort;
}

void MPXbee::getChannel( byte *channel )
{
  //Read operation channel of xbee module
  channel[ 0 ] = _opChannel;
}

void MPXbee::getExtendedPAN( int *PAN )
{
  //Read extended PAN
  PAN[ 0 ] = _extPAN;
}

void MPXbee::getOperationPAN( int *PAN )
{
  //Read operation PAN
  PAN[ 0 ] = _opPAN;
}

void MPXbee::getSrcAddress( unsigned int *srcAddress )
{
  //Read network address
  srcAddress[ 0 ] = _nwkAddress;
}

void MPXbee::getDestAddress( unsigned int *destAddressH, unsigned int *destAddressL )
{
  //Read destination address
  destAddressH[ 0 ] = _destAddressH;
  destAddressL[ 0 ] = _destAddressL;
}

boolean MPXbee::setExtendedPAN( int *PAN )
{
  //Declarations
  String paramValue = "";

  //Write extended PAN of xbee module
  paramValue = String( PAN[ 0 ], HEX );
  while( !setExtPAN( &paramValue ) )
  {
    delay( random( 10, 500 ) );
  }

  _extPAN = ( int )strtol( paramValue.c_str(), NULL , 16 );
  return true;
}

boolean MPXbee::readData( char *c )
{
  switch( _serialPort )
  {
    case MPXBEE_SERIALPORT_0:
	  while( Serial.available() == 0 )
	  {}
      c[ 0 ] = Serial.read();
      return true;
    break;
    case MPXBEE_SERIALPORT_1:
      while( Serial1.available() == 0 )
	  {}
      c[ 0 ] = Serial1.read();
      return true;
    break;
    case MPXBEE_SERIALPORT_2:
      while( Serial2.available() == 0 )
	  {}
      c[ 0 ] = Serial2.read();
      return true;
    break;
    case MPXBEE_SERIALPORT_3:
      while( Serial3.available() == 0 )
	  {}
      c[ 0 ] = Serial3.read();
      return true;
    break;
  }

  return false;
}

void MPXbee::readData( byte str_length, String *str )
{
	byte num_bytes_read;
	char c;
	
	*str = String( "" );
	if( str_length > 0 )
	{
		num_bytes_read = 0;
		while( num_bytes_read < str_length )
		{
			this->readData( &c );
			*str += String( c );
			num_bytes_read++;
		}
	}
	else {
		num_bytes_read = 0;
		while (this->readData( &c ) && c != '\n'){
			  *str += String( c );
		          num_bytes_read++;
		}
	}
}

void MPXbee::writeData( char c )
{
  switch( _serialPort )
  {
    case MPXBEE_SERIALPORT_0:
    Serial.write( c );
    break;
    case MPXBEE_SERIALPORT_1:
    Serial1.write( c );
    break;
    case MPXBEE_SERIALPORT_2:
    Serial2.write( c );
    break;
    case MPXBEE_SERIALPORT_3:
    Serial3.write( c );
    break;
  }
}

void MPXbee::writeData( byte str_length, String str )
{
  byte num_bytes;
  
  num_bytes = min( str_length, str.length() );
  for( byte i = 0; i < num_bytes; i++ )
  {
    this->writeData( str.charAt( i ) );
  }
}

boolean MPXbee::getAssoc( void )
{
  //Declarations
  String paramValue = "";
  String assocResult = "";
  //Read association indication
  if( !readParameter( MPXBEE_ASSOCIND, &paramValue ) )
  {
    return false;
  }
  
  //Create association indication in xx format
  switch( paramValue.length() )
  {
    case 1:
    assocResult = "0" + paramValue;
    break;
    default:
    break;
  }
  
  //Verify association indication result
  if( assocResult == "00" )
  {
    return true;
  }
  else
  {
    return false;
  }
}

boolean MPXbee::getOpChannel( String *opChannel )
{
  //Declarations
  String paramValue = "";
  
  //Read operating channel
  if( !readParameter( MPXBEE_OPCHANNEL, &paramValue ) )
  {
    return false;
  }
  
  //Create operating channel in xx format
  switch( paramValue.length() )
  {
    case 1:
    *opChannel = "0" + paramValue;
    break;
    case 2:
    *opChannel = paramValue;
    break;
    default:
    break;
  }
  
  return true;
}

boolean MPXbee::getExtPAN( String *extPAN )
{
  //Declarations
  String paramValue = "";
  
  //Read extended PAN Id
  if( !readParameter( MPXBEE_EXTPANID, &paramValue ) )
  {
    return false;
  }
  
  //Create extended PAN Id in xxxxxxxxxxxxxxxx format
  switch( paramValue.length() )
  {
    case 1:
    *extPAN = "000000000000000" + paramValue;
    break;
    case 2:
    *extPAN = "00000000000000" + paramValue;
    break;
    case 3:
    *extPAN = "0000000000000" + paramValue;
    break;
    case 4:
    *extPAN = "000000000000" + paramValue;
    break;
    case 5:
    *extPAN = "00000000000" + paramValue;
    break;
    case 6:
    *extPAN = "0000000000" + paramValue;
    break;
    case 7:
    *extPAN = "000000000" + paramValue;
    break;
    case 8:
    *extPAN = "00000000" + paramValue;
    break;
    case 9:
    *extPAN = "0000000" + paramValue;
    break;
    case 10:
    *extPAN = "000000" + paramValue;
    break;
    case 11:
    *extPAN = "00000" + paramValue;
    break;
    case 12:
    *extPAN = "0000" + paramValue;
    break;
    case 13:
    *extPAN = "000" + paramValue;
    break;
    case 14:
    *extPAN = "00" + paramValue;
    break;
    case 15:
    *extPAN = "0" + paramValue;
    break;
    case 16:
    *extPAN = paramValue;
    break;
    default:
    break;
  }
  
  return true;
}

boolean MPXbee::getOpPAN( String *opPAN )
{
  //Declarations
  String paramValue = "";
  
  //Read operating extended PAN Id
  if( !readParameter( MPXBEE_OPEXTPANID, &paramValue ) )
  {
    return false;
  }
  
  //Create operating extended PAN Id in xxxxxxxxxxxxxxxx format
  switch( paramValue.length() )
  {
    case 1:
    *opPAN = "000000000000000" + paramValue;
    break;
    case 2:
    *opPAN = "00000000000000" + paramValue;
    break;
    case 3:
    *opPAN = "0000000000000" + paramValue;
    break;
    case 4:
    *opPAN = "000000000000" + paramValue;
    break;
    case 5:
    *opPAN = "00000000000" + paramValue;
    break;
    case 6:
    *opPAN = "0000000000" + paramValue;
    break;
    case 7:
    *opPAN = "000000000" + paramValue;
    break;
    case 8:
    *opPAN = "00000000" + paramValue;
    break;
    case 9:
    *opPAN = "0000000" + paramValue;
    break;
    case 10:
    *opPAN = "000000" + paramValue;
    break;
    case 11:
    *opPAN = "00000" + paramValue;
    break;
    case 12:
    *opPAN = "0000" + paramValue;
    break;
    case 13:
    *opPAN = "000" + paramValue;
    break;
    case 14:
    *opPAN = "00" + paramValue;
    break;
    case 15:
    *opPAN = "0" + paramValue;
    break;
    case 16:
    *opPAN = paramValue;
    break;
    default:
    break;
  }
  
  return true;
}

boolean MPXbee::getNwkAddress( String *nwkAddr )
{
  //Declarations
  String paramValue = "";
  
  //Read network address
  if( !readParameter( MPXBEE_NWKADDR, &paramValue ) )
  {
    return false;
  }
  
  //Create network address in xxxx format
  switch( paramValue.length() )
  {
    case 1:
    *nwkAddr = "000" + paramValue;
    break;
    case 2:
    *nwkAddr = "00" + paramValue;
    break;
    case 3:
    *nwkAddr = "0" + paramValue;
    break;
    case 4:
    *nwkAddr = paramValue;
    break;
    default:
    break;
  }
  
  return true;
}

boolean MPXbee::getDestinationAddress( String *destAddrH, String *destAddrL )
{
  //Declarations
  String paramValue;
  
  //Read destination address high parameter
  if( !readParameter( MPXBEE_DESTADDRH, &paramValue ) )
  {
    return false;
  }

  //Convert destination address high string to xxxxxxxx format
  switch( paramValue.length() )
  {
    case 1:
    *destAddrH = "0000000" + paramValue;
    break;
    case 2:
    *destAddrH = "000000" + paramValue;
    break;
    case 3:
    *destAddrH = "00000" + paramValue;
    break;
    case 4:
    *destAddrH = "0000" + paramValue;
    break;
    case 5:
    *destAddrH = "000" + paramValue;
    break;
    case 6:
    *destAddrH = "00" + paramValue;
    break;
    case 7:
    *destAddrH = "0" + paramValue;
    break;
    case 8:
    *destAddrH = paramValue;
    break;
    default:
    break;
  }
    
  //Read destination address low parameter
  if( !readParameter( MPXBEE_DESTADDRL, &paramValue ) )
  {
    return false;
  }
  
  //Convert destination address low string to xxxxxxxx format
  switch( paramValue.length() )
  {
    case 1:
    *destAddrL = "0000000" + paramValue;
    break;
    case 2:
    *destAddrL = "000000" + paramValue;
    break;
    case 3:
    *destAddrL = "00000" + paramValue;
    break;
    case 4:
    *destAddrL = "0000" + paramValue;
    break;
    case 5:
    *destAddrL = "000" + paramValue;
    break;
    case 6:
    *destAddrL = "00" + paramValue;
    break;
    case 7:
    *destAddrL = "0" + paramValue;
    break;
    case 8:
    *destAddrL = paramValue;
    break;
    default:
    break;
  }
  
  return true;
}

boolean MPXbee::setExtPAN( String *extPAN )
{
  //Write extended PAN Id
  if( !writeParameter( MPXBEE_EXTPANID, extPAN ) )
  {
    return false;
  }

  return true;
}

boolean MPXbee::readParameter( byte Id, String *Value )
{ 
  //Enter xbee module to command mode
  if( enterCommandMode() )
  {
    //Verify Id parameter
    switch( Id )
    {
      case MPXBEE_DESTADDRH:
      switch( _serialPort )
      {
        case MPXBEE_SERIALPORT_0:
        Serial.print("ATDH\r");
        break;
        case MPXBEE_SERIALPORT_1:
        Serial1.print("ATDH\r");
        break;
        case MPXBEE_SERIALPORT_2:
        Serial2.print("ATDH\r");
        break;
        case MPXBEE_SERIALPORT_3:
        Serial3.print("ATDH\r");
        break;
      }
      break;
      case MPXBEE_DESTADDRL:
      switch( _serialPort )
      {
        case MPXBEE_SERIALPORT_0:
        Serial.print("ATDL\r");
        break;
        case MPXBEE_SERIALPORT_1:
        Serial1.print("ATDL\r");
        break;
        case MPXBEE_SERIALPORT_2:
        Serial2.print("ATDL\r");
        break;
        case MPXBEE_SERIALPORT_3:
        Serial3.print("ATDL\r");
        break;
      }
      break;
      case MPXBEE_NWKADDR:
      switch( _serialPort )
      {
        case MPXBEE_SERIALPORT_0:
        Serial.print("ATMY\r");
        break;
        case MPXBEE_SERIALPORT_1:
        Serial1.print("ATMY\r");
        break;
        case MPXBEE_SERIALPORT_2:
        Serial2.print("ATMY\r");
        break;
        case MPXBEE_SERIALPORT_3:
        Serial3.print("ATMY\r");
        break;
      }
      break;
      case MPXBEE_OPCHANNEL:
      switch( _serialPort )
      {
        case MPXBEE_SERIALPORT_0:
        Serial.print("ATCH\r");
        break;
        case MPXBEE_SERIALPORT_1:
        Serial1.print("ATCH\r");
        break;
        case MPXBEE_SERIALPORT_2:
        Serial2.print("ATCH\r");
        break;
        case MPXBEE_SERIALPORT_3:
        Serial3.print("ATCH\r");
        break;
      }
      break;
      case MPXBEE_SCANCHANNEL:
      switch( _serialPort )
      {
        case MPXBEE_SERIALPORT_0:
        Serial.print("ATSC\r");
        break;
        case MPXBEE_SERIALPORT_1:
        Serial1.print("ATSC\r");
        break;
        case MPXBEE_SERIALPORT_2:
        Serial2.print("ATSC\r");
        break;
        case MPXBEE_SERIALPORT_3:
        Serial3.print("ATSC\r");
        break;
      }
      break;
      case MPXBEE_OPEXTPANID:
      switch( _serialPort )
      {
        case MPXBEE_SERIALPORT_0:
        Serial.print("ATOP\r");
        break;
        case MPXBEE_SERIALPORT_1:
        Serial1.print("ATOP\r");
        break;
        case MPXBEE_SERIALPORT_2:
        Serial2.print("ATOP\r");
        break;
        case MPXBEE_SERIALPORT_3:
        Serial3.print("ATOP\r");
        break;
      }
      break;
      case MPXBEE_EXTPANID:
      switch( _serialPort )
      {
        case MPXBEE_SERIALPORT_0:
        Serial.print("ATID\r");
        break;
        case MPXBEE_SERIALPORT_1:
        Serial1.print("ATID\r");
        break;
        case MPXBEE_SERIALPORT_2:
        Serial2.print("ATID\r");
        break;
        case MPXBEE_SERIALPORT_3:
        Serial3.print("ATID\r");
        break;
      }
      break;
      case MPXBEE_ASSOCIND:
      switch( _serialPort )
      {
        case MPXBEE_SERIALPORT_0:
        Serial.print("ATAI\r");
        break;
        case MPXBEE_SERIALPORT_1:
        Serial1.print("ATAI\r");
        break;
        case MPXBEE_SERIALPORT_2:
        Serial2.print("ATAI\r");
        break;
        case MPXBEE_SERIALPORT_3:
        Serial3.print("ATAI\r");
        break;
      }
      break;
      default:
      break;
    }
    
    //Read parameter value
    readCommand( Value );
    
    //Exit xbee module from command mode
    if( exitCommandMode() )
    {
      return true;
    }
  }

  return false;
}

boolean MPXbee::writeParameter( byte Id, String *Value )
{
  //Declarations
  String Cmd = "";

  //Enter xbee module to command mode
  if( enterCommandMode() )
  {
    //Verify Id parameter
    switch( Id )
    {
      case MPXBEE_EXTPANID:
      Cmd = String( "ATID " ) + Value[ 0 ] + String( "\r" );
      switch( _serialPort )
      {
        case MPXBEE_SERIALPORT_0:
        Serial.print( Cmd );
        break;
        case MPXBEE_SERIALPORT_1:
        Serial1.print( Cmd );
        break;
        case MPXBEE_SERIALPORT_2:
        Serial2.print( Cmd );
        break;
        case MPXBEE_SERIALPORT_3:
        Serial3.print( Cmd );
        break;
      }
      break;
      default:
      break;
    }

    //Read and Verify command
    readCommand( &Cmd );
    if( Cmd == "OK" )
    {
      //Write in permanent memory
      Cmd = String( "ATWR\r" );
      switch( _serialPort )
      {
        case MPXBEE_SERIALPORT_0:
        Serial.print( Cmd );
        delay( 500 );
        break;
        case MPXBEE_SERIALPORT_1:
        Serial1.print( Cmd );
        delay( 500 );
        break;
        case MPXBEE_SERIALPORT_2:
        Serial2.print( Cmd );
        delay( 500 );
        break;
        case MPXBEE_SERIALPORT_3:
        Serial3.print( Cmd );
        delay( 500 );
        break;
      }

      //Read command
      readCommand( &Cmd );
      if( Cmd == "OK" )
      {
        //Exit xbee module from command mode
        if( exitCommandMode() )
        {
          return true;
        }
      }
    }
  }
  
  return false;
}

boolean MPXbee::enterCommandMode( void )
{
  //Declarations
  String Cmd = "";

  //Delay Guard Time
  delay( 1000 );
  
  //Send Command Sequence Character
  switch( _serialPort )
  {
    case MPXBEE_SERIALPORT_0:
    Serial.write( '+' );
    Serial.write( '+' );
    Serial.write( '+' );
    break;
    case MPXBEE_SERIALPORT_1:
    Serial1.write( '+' );
    Serial1.write( '+' );
    Serial1.write( '+' );
    break;
    case MPXBEE_SERIALPORT_2:
    Serial2.write( '+' );
    Serial2.write( '+' );
    Serial2.write( '+' );
    break;
    case MPXBEE_SERIALPORT_3:
    Serial3.write( '+' );
    Serial3.write( '+' );
    Serial3.write( '+' );
    break;
  }
  
  //Delay Guard Time
  delay( 1000 );
  //Read and verify command
  if( readCommand( &Cmd ) )
  {
    if( Cmd == "OK" )
    {
      return true;
    }
  }

  return false;
}

boolean MPXbee::exitCommandMode( void )
{
  //Declarations
  String Cmd = "";

  //Send exit command
  switch( _serialPort )
  {
    case MPXBEE_SERIALPORT_0:
    Serial.print( "ATCN\r" );
    break;
    case MPXBEE_SERIALPORT_1:
    Serial1.print( "ATCN\r" );
    break;
    case MPXBEE_SERIALPORT_2:
    Serial2.print( "ATCN\r" );
    break;
    case MPXBEE_SERIALPORT_3:
    Serial3.print( "ATCN\r" );
    break;
  }
  
  //Read and verify command
  if( readCommand( &Cmd ) )
  {
    if( Cmd == "OK" )
    {
      return true;
    }
  }

  return false;
}

boolean MPXbee::readCommand( String *Cmd )
{
  //declarations
  char c;
  long timeStart;
  long timeDif;
  
  //Initializations
  c = 0x00;
  *Cmd = "";
  timeDif = 0;
  timeStart = millis();
  
  //Read bytes until end of command
  //End of command character non included
  switch( _serialPort )
  {
    case MPXBEE_SERIALPORT_0:
    while( ( c != '\r' ) && (timeDif < MPXBEE_SERIAL_TIMEOUT ) )
    {
      if( Serial.available() )
      {
        c = Serial.read();
        if( c != '\r' )
        {
          *Cmd += c;
        }
      
        //Update timeout variables
        timeStart = millis();
      }
    
      //Update timeout variables
      timeDif = millis() - timeStart;
    }
    break;
    case MPXBEE_SERIALPORT_1:
    while( ( c != '\r' ) && (timeDif < MPXBEE_SERIAL_TIMEOUT ) )
    {
      if( Serial1.available() )
      {
        c = Serial1.read();
        if( c != '\r' )
        {
          *Cmd += c;
        }
      
        //Update timeout variables
        timeStart = millis();
      }
    
      //Update timeout variables
      timeDif = millis() - timeStart;
    }
    break;
    case MPXBEE_SERIALPORT_2:
    while( ( c != '\r' ) && (timeDif < MPXBEE_SERIAL_TIMEOUT ) )
    {
      if( Serial2.available() )
      {
        c = Serial2.read();
        if( c != '\r' )
        {
          *Cmd += c;
        }
      
        //Update timeout variables
        timeStart = millis();
      }
    
      //Update timeout variables
      timeDif = millis() - timeStart;
    }
    break;
    case MPXBEE_SERIALPORT_3:
    while( ( c != '\r' ) && (timeDif < MPXBEE_SERIAL_TIMEOUT ) )
    {
      if( Serial3.available() )
      {
        c = Serial3.read();
        if( c != '\r' )
        {
          *Cmd += c;
        }
      
        //Update timeout variables
        timeStart = millis();
      }
    
      //Update timeout variables
      timeDif = millis() - timeStart;
    }
    break;
  }
  
  if( c == '\r' )
  {
    return true;
  }
  else
  {
    return false;
  }
}
