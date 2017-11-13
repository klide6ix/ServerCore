#pragma once

#include "TestProtocol.h"

#include "..\Utility\BufferSerializer.h"

inline bool decode_CS_ECHO_TEST_REQ( BufferSerializer& serializer, PCK_CS_ECHO_TEST_REQ& pck );
inline static bool decode_SC_ECHO_TEST_ACK( BufferSerializer& serializer, PCK_SC_ECHO_TEST_ACK& pck );
inline static bool decode_CS_DB_TEST_REQ( BufferSerializer& serializer, PCK_CS_DB_TEST_REQ& pck );
inline static bool decode_SC_PING( BufferSerializer& serializer, PCK_SC_PING& pck );
inline static bool decode_CS_PONG( BufferSerializer& serializer, PCK_CS_PONG& pck );

#include "TestProtocolDecode.inl"
