#pragma once
#pragma pack ( push, 1 )

#include "ProtocolDefine.h"

#define PROTOCOL_VERSION 1024

enum EnumTestProtocol
{
	CS_ECHO_TEST_REQ = 0,
	SC_ECHO_TEST_ACK,
	CS_DB_TEST_REQ,
	SC_PING,
	CS_PONG,
};

struct PCK_CS_ECHO_TEST_REQ : public PACKET_HEADER
{
	int data1_ = 0;
	char  data2_[2048] = {};
	std::vector<float> data3_;

	PCK_CS_ECHO_TEST_REQ() : PACKET_HEADER()
	{
		setProtocol( CS_ECHO_TEST_REQ);
		setSize( sizeof( PCK_CS_ECHO_TEST_REQ ) );
	}
};

struct PCK_SC_ECHO_TEST_ACK : public PACKET_HEADER
{
	int data1_ = 0;
	char  data2_[2048] = {};
	std::vector<float> data3_;

	PCK_SC_ECHO_TEST_ACK() : PACKET_HEADER()
	{
		setProtocol( SC_ECHO_TEST_ACK);
		setSize( sizeof( PCK_SC_ECHO_TEST_ACK ) );
	}
};

struct PCK_CS_DB_TEST_REQ : public PACKET_HEADER
{

	PCK_CS_DB_TEST_REQ() : PACKET_HEADER()
	{
		setProtocol( CS_DB_TEST_REQ);
		setSize( sizeof( PCK_CS_DB_TEST_REQ ) );
	}
};

struct PCK_SC_PING : public PACKET_HEADER
{
	unsigned int sequence_ = 0;

	PCK_SC_PING() : PACKET_HEADER()
	{
		setProtocol( SC_PING);
		setSize( sizeof( PCK_SC_PING ) );
	}
};

struct PCK_CS_PONG : public PACKET_HEADER
{
	unsigned int sequence_ = 0;

	PCK_CS_PONG() : PACKET_HEADER()
	{
		setProtocol( CS_PONG);
		setSize( sizeof( PCK_CS_PONG ) );
	}
};

#pragma pack ( pop )
