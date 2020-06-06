//MPXBee
//Version: 2.0

#ifndef MPXbee_h
#define MPXbee_h

#include "Arduino.h"

#define MPXBEE_SERIALPORT_UNDEF		0xFF
#define MPXBEE_SERIALPORT_0			0x00
#define MPXBEE_SERIALPORT_1 		0x01
#define MPXBEE_SERIALPORT_2 		0x02
#define MPXBEE_SERIALPORT_3 		0x03
#define MPXBEE_SERIAL_TIMEOUT		500

#define MPXBEE_CHANNEL_UNDEF		0xFF
#define MPXBEE_PAN_UNDEF			0xFFFF
#define MPXBEE_NWKADDRESS_UNDEF		0xFFFF
#define MPXBEE_DESTADDRESSH_UNDEF	0xFFFF
#define MPXBEE_DESTADDRESSL_UNDEF	0xFFFF

#define MPXBEE_DESTADDRH			1
#define MPXBEE_DESTADDRL   			2
#define MPXBEE_NWKADDR     			3
#define MPXBEE_OPCHANNEL   			4
#define MPXBEE_SCANCHANNEL			5
#define MPXBEE_EXTPANID    			6
#define MPXBEE_OPEXTPANID  			7
#define MPXBEE_ASSOCIND    			8

class MPXbee
{
  public:
    MPXbee();
    void SerialInit( byte serialPort );
    boolean Init( byte serialPort );
    void getSerialPort( byte *serialPort );
    void getChannel( byte *channel );
    void getExtendedPAN( int *PAN );
    void getOperationPAN( int *PAN );
    void getSrcAddress( unsigned int *srcAddress );
    void getDestAddress( unsigned int *destAddressH, unsigned int *destAddressL );
    boolean setExtendedPAN( int *PAN );
    boolean readData( char *c );
	void readData( byte str_length, String *str );
    void writeData( char c );
	void writeData( byte str_length, String str );
  private:
    byte _serialPort;
    byte _opChannel;
    int _extPAN;
    int _opPAN;
    unsigned int _nwkAddress;
    unsigned int _destAddressH;
    unsigned int _destAddressL;
    boolean getAssoc( void );
    boolean getOpChannel( String *opChannel );
    boolean getExtPAN( String *extPAN );
    boolean getOpPAN( String *opPAN );
    boolean getNwkAddress( String *nwkAddr );
    boolean getDestinationAddress( String *destAddrH, String *destAddrL );
    boolean setExtPAN( String *extPAN );
    boolean readParameter( byte Id, String *Value );
    boolean writeParameter( byte Id, String *Value );
    boolean enterCommandMode( void );
    boolean exitCommandMode ( void );
    boolean readCommand( String *Cmd );
};

#endif
