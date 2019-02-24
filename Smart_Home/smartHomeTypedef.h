#pragma once

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

#define MESSAGE_PACKET_SIZE	100
//enum MessagePacketState {
//	FIRST_BYTE = 0,
//	SENDER,
//	RECEIVER,
//	FUNCTION,
//	DATA
//};
enum MessageCtrFunction {
	HEARTBEAT = 0,
	FIND_DEVICE,
	LIGHT,
	FAN,
	HUMITURE = 4,
	WIFI_CONFIG_SET = 253,
	WIFI_CONFIG = 254,
	DEVICE_ERROS = 255
};

struct MessagePacketStruct {
	uint16 sender;
	uint16 receiver;
	uint8 function_word;
	uint8 data[95];
};
union MessagePacketUnion {
	struct MessagePacketStruct messagePacket;
	uint8 p_buff[100];	//Э��涨�������Ϊ100
};

struct DeviceInfo {
	uint16 devicesNumber;	//�豸��
	sockaddr_in address;	//��ӦIP
	uint8 functionWorld;	//��Ӧ����
	uint8 isExist;			//�Ƿ����
	uint8 loosTimes;		//���˶��ٸ�ʱ�䵥λû����Ӧ
};

