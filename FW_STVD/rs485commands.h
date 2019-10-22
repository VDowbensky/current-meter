#ifndef _COMMANDS_H_
#define _COMMANDS_H_

//RS485 commands

#define C_READONCE 		0x06 //request data
#define C_READCONT		0x07 //autoreport data

#define TC_ENTRY				0x80
#define TC_RD_SN				0x81
#define TC_RD_CONFIG		0x82 //send fake config byte
#define TC_WR_CONFIG		0x83 //fake, answer OK & nothing to do
#define TC_RD_OFFSET		0x84 //offset12 only
#define TC_WR_OFFSET		0x85 //offset12 only
#define TC_RD_VTUNE			0x86
#define TC_WR_VTUNE			0x87
#define TC_WRDAC12			0x88
#define TC_WRDAC8				0x89 //fake, answer OK & nothing to do
#define TC_RD_GAIN			0x8A
#define TC_WR_GAIN			0x8B
#define TC_WR_DEFAULT		0x8C //switch off lister mode, set gain to 1
#define TC_WR_SN				0x8D
#define TC_RESET_SN			0x8E
#define TC_EXIT					0x8F

#define TXPREFIX				0x31
#define RXPREFIX				0x3E
#define TUNEPREFIX			0x3F
#define IDENT						0x01
#define CRCPOLY					0x18
#define RESP_OK					0
#define RESP_ERROR			1


#endif

