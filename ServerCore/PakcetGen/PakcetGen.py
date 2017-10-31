import json
import os
import shutil

ServerList = [ "..\GameServer" ]
ClientList = [ "..\TestClient",
               "..\..\VoltStrikeClient\Source\VoltStrike" ]

def CopyToServer( FileName ) :

	for AbsPath in ServerList :
		ServerPath = os.path.abspath( AbsPath )
		dstFile = os.path.join( ServerPath, FileName )
		#print( dstFile )

		srcFile = os.path.join( ".\\", FileName )
		#print( srcFile )

		if os.path.exists(dstFile):
			os.remove(dstFile)
		shutil.copy2( srcFile, dstFile )

def CopyToClient( FileName ) :
	for AbsPath in ClientList :
		ClientPath = os.path.abspath( AbsPath )
		dstFile = os.path.join( ClientPath, FileName )
		#print( dstFile )

		srcFile = os.path.join( ".\\", FileName )
		#print( srcFile )

		if os.path.exists(dstFile):
			os.remove(dstFile)
		shutil.copy2( srcFile, dstFile )

def CreateProtocolFile( jsonName ) :
    print("CreateProtocolFile Start")
    protocolFile = open( jsonName, 'r' )

    fileData = protocolFile.read()
    #print(fileData)

    protocolFile.close()

    protocolDict = json.loads(fileData)
    #print(protocolDict)

    fileName = jsonName[ : jsonName.find( '.' ) ]
    fileName = fileName + ".h"
    
    # 프로토콜 버전
    PROTOCOL_VERSION = 1000
    orgFile = open( fileName, 'r' )
    orgData = orgFile.readlines()
    for orgLine in orgData :
        if orgLine.find("PROTOCOL_VERSION") != -1 :
            thisVersion = orgLine[ 25 : ]
            PROTOCOL_VERSION = int(thisVersion) + 1
        
    orgFile.close()
    
    resultFile = open( fileName, 'w' )

    # 설정
    resultFile.write("#pragma once\n")
    resultFile.write("#pragma pack ( push, 1 )\n\n")

    resultFile.write("#include \"ProtocolDefine.h\"\n\n")

    resultFile.write("#define PROTOCOL_VERSION " + str(PROTOCOL_VERSION) + "\n\n")

    # 프로토콜 Enum
    resultFile.write("enum Enum" + fileName[ : fileName.find( '.' ) ] + "\n")
    resultFile.write("{\n")

    for k in protocolDict.keys() :
        dictLine = str(protocolDict[k])
        dictLine = dictLine.replace('\'', '\"')
        dictLine = dictLine.replace('None', 'null')
        #print( dictLine )
        protocolTypeDict = json.loads( dictLine )

        linedata = "\t" + k + ",\n"

        for ptdct in protocolTypeDict:
            if 'INDEX' in ptdct :
                linedata = "\t" + k + " = " + ptdct['INDEX'] + ",\n"
                break

        resultFile.write(linedata)

    resultFile.write("};\n\n")

    # 구조체
    for k in protocolDict.keys() :
        dictLine = str(protocolDict[k])
        dictLine = dictLine.replace('\'', '\"')
        dictLine = dictLine.replace('None', 'null')
        protocolTypeDict = json.loads( dictLine )

        resultFile.write("struct PCK_" + k + " : public PACKET_HEADER\n")
        resultFile.write("{\n")

        # 멤버 변수
        for ptdct in protocolTypeDict:
            if type(ptdct['TYPE']) != str :
                continue

            # 배열 처리
            if ptdct['TYPE'].find( '[' ) == -1 :
                linedata = "\t" + ptdct['TYPE'] + " " + ptdct['NAME']
                if 'DEFAULT' in ptdct :
                    linedata += " = " + ptdct['DEFAULT']
                linedata += ";\n"
            else :
                arrayType = ptdct['TYPE'][ : ptdct['TYPE'].find( '[' ) ]
                arrayEnd = ptdct['TYPE'][ ptdct['TYPE'].find( '[' ) : ]
                linedata = "\t" + arrayType + " " + ptdct['NAME'] + arrayEnd
                if 'DEFAULT' in ptdct :
                    linedata += " = " + ptdct['DEFAULT']
                linedata += ";\n"

            resultFile.write(linedata)

        # 생성자
        resultFile.write("\n")
        resultFile.write("\tPCK_" + k + "() : PACKET_HEADER()\n")
        resultFile.write("\t{\n")
        resultFile.write("\t\tsetProtocol( " + k + ");\n")
        resultFile.write("\t\tsetSize( sizeof( PCK_" + k + " ) );\n")
        resultFile.write("\t}\n")
        resultFile.write("};\n\n")

    resultFile.write("#pragma pack ( pop )\n")

    resultFile.close()

    CopyToServer( fileName )
    CopyToClient( fileName )

    print("CreateProtocolFile End")
#CreateProtocolFile end

def ParseFunctionLine( resultFile, protocolName, dictLine, isDefine ) :
    dictLine = dictLine.replace('\'', '\"')
    dictLine = dictLine.replace('None', 'null')
    #print(dictLine)
    protocolTypeDict = json.loads( dictLine )

    #print( protocolName, protocolTypeDict )

    resultFile.write("inline static bool encode_" + protocolName + "( BufferSerializer& serializer")

    # 파라미터
    for ptdct in protocolTypeDict :
        if type(ptdct['TYPE']) != str :
            continue

        if ptdct == protocolTypeDict[0] :
            resultFile.write( "," )

	# 배열 처리
        if ptdct['TYPE'].find( '[' ) != -1 :
            arrayType = ptdct['TYPE'][ : ptdct['TYPE'].find( '[' ) ]
            arrayLength = ptdct['TYPE'][ ptdct['TYPE'].find( '[' ) + 1 : ptdct['TYPE'].find( ']' ) ]
            arrayLengthName = ptdct['NAME'][ : -1] + "Size_"
            resultFile.write( " const " + arrayType + "* " + ptdct['NAME'] + ", unsigned short " + arrayLengthName )

	# 벡터
        elif ptdct['TYPE'].find( 'std::vector<' ) != -1 :
            resultFile.write( " IEncodeIterator* " + ptdct['NAME'] )

	# 일반 타입
        else :
            resultFile.write( " const " + ptdct['TYPE'] + "& " + ptdct['NAME'] )

        if ptdct != protocolTypeDict[-1] :
            resultFile.write( "," )

    resultFile.write(" )")
    if isDefine == True :
        resultFile.write(";")
    resultFile.write("\n")

# ParseFunctionLine End

def CreateEncoderFile( jsonName ) :

    print("CreateEncoderFile Start")

    protocolFile = open( jsonName, 'r' )

    fileData = protocolFile.read()
    #print(fileData)

    protocolFile.close()

    protocolDict = json.loads(fileData)
    #print(protocolDict)

    fileName = jsonName[ : jsonName.find( '.' ) ]
    fileName = fileName + "Encode.h"
    resultFile = open( fileName, 'w' )

    # 설정
    resultFile.write("#pragma once\n\n")
    resultFile.write("#include \"ClientProtocol.h\"\n\n")
    resultFile.write("#include \"..\\Utility\\BufferSerializer.h\"\n\n")

    resultFile.write("class IEncodeIterator\n")
    resultFile.write("{\n")
    resultFile.write("public:\n")
    resultFile.write("\tvirtual void begin() = 0;\n")
    resultFile.write("\tvirtual void next() = 0;\n")
    resultFile.write("\tvirtual bool hasNext() = 0;\n")
    resultFile.write("\tvirtual bool fill( BufferSerializer& serializer ) = 0;\n")
    resultFile.write("};\n\n")

    for k in protocolDict.keys() :
        ParseFunctionLine( resultFile, k, str(protocolDict[k]), True )

    resultFile.write("\n#include \"ClientProtocolEncode.inl\"\n")
    resultFile.close()

    CopyToServer( fileName )

    print("CreateEncoderFile End")
#CreateEncoderFile end


def CreateEncoderInlFile( jsonName ) :

    print("CreateEncoderInlFile Start")

    protocolFile = open( jsonName, 'r' )

    fileData = protocolFile.read()
    #print(fileData)

    protocolFile.close()

    protocolDict = json.loads(fileData)
    #print(protocolDict)

    fileName = jsonName[ : jsonName.find( '.' ) ]
    fileName = fileName + "Encode.inl"
    resultFile = open( fileName, 'w' )

    for k in protocolDict.keys() :

        # 정의
        ParseFunctionLine( resultFile, k, str(protocolDict[k]), False )

        dictLine = str(protocolDict[k])
        dictLine = dictLine.replace('\'', '\"')
        dictLine = dictLine.replace('None', 'null')
        #print( dictLine )
        protocolTypeDict = json.loads( dictLine )

        # 내용
        resultFile.write("{\n")
        resultFile.write("\tPACKET_HEADER* header = serializer.getTypePointer<PACKET_HEADER>();\n")
        resultFile.write("\theader->protocol_ = " + k + ";\n\n")

        for ptdct in protocolTypeDict :
            if type(ptdct['TYPE']) != str :
                continue

            # 배열 처리
            if ptdct['TYPE'].find( '[' ) != -1 :
                arrayType = ptdct['TYPE'][ : ptdct['TYPE'].find( '[' ) ]
                arrayLength = ptdct['TYPE'][ ptdct['TYPE'].find( '[' ) + 1 : ptdct['TYPE'].find( ']' ) ]
                arrayLengthName = ptdct['NAME'][ : -1] + "Size_"
                resultFile.write( "\tserializer.put_data( " + ptdct['NAME'] + ", " + arrayLengthName + " );\n" );

            # 벡터
            elif ptdct['TYPE'].find( 'std::vector<' ) != -1 :
                resultFile.write( "\tunsigned short* count = serializer.getTypePointer<unsigned short>();\n" )

                resultFile.write( "\tif( count == nullptr )\n" )
                resultFile.write( "\t\treturn false;\n\n" )

                resultFile.write( "\tfor( " + ptdct['NAME'] + "->begin(); " + ptdct['NAME'] + "->hasNext(); " + ptdct['NAME'] + "->next() )\n" )
                resultFile.write( "\t{\n" )
                resultFile.write( "\t\t" + ptdct['NAME'] + "->fill( serializer );\n" )
                resultFile.write( "\t\t++(*count);\n" )
                resultFile.write( "\t}\n" )

            # 일반 타입
            else :
                resultFile.write( "\tserializer.put_data( " + ptdct['NAME'] + " );\n" )

            # 마지막 라인일 경우 줄바꿈 추가
            if ptdct == protocolTypeDict[-1] :
                resultFile.write( "\n" )
	
        resultFile.write("\theader->size_ = static_cast<unsigned short>( serializer.getSize() );\n")

        resultFile.write("\n\treturn true;\n")
        resultFile.write("}\n")

    resultFile.close()

    CopyToServer( fileName )

    print("CreateEncoderInlFile End")
#CreateEncoderInlFile end

def CreateDecoderFile( jsonName ) :
    print("CreateDecoderFile Start")

    protocolFile = open( jsonName, 'r' )

    fileData = protocolFile.read()

    protocolFile.close()

    protocolDict = json.loads(fileData)

    fileName = jsonName[ : jsonName.find( '.' ) ]
    fileName = fileName + "Decode.h"
    resultFile = open( fileName, 'w' )

    resultFile.write("#pragma once\n")
    resultFile.write("#include <map>\n")

    resultFile.write("#include \"ClientProtocol.h\"\n\n")

    resultFile.write("using DecodeFunction_t = bool (*)( PACKET_HEADER*& pck, char* buffer );\n")
    resultFile.write("class ClientProtocolDecoder\n")
    resultFile.write("{\n")
    resultFile.write("\tstd::map<EnumClientProtocol, DecodeFunction_t> decodeFunction_;\n\n")

    resultFile.write("public:\n")
    resultFile.write("\tClientProtocolDecoder();\n")
    resultFile.write("\t~ClientProtocolDecoder();\n\n")

    resultFile.write("\tbool DecodeClientProtocol( EnumClientProtocol protocol, PACKET_HEADER*& pck, char* buffer );\n")
    resultFile.write("};\n")


    resultFile.close()

    CopyToClient( fileName )
    
    print("CreateDecoderFile End")
#CreateDecoderFile end

def CreateDecoderCppFile( jsonName ) :
    print("CreateDecoderCppFile Start")

    protocolFile = open( jsonName, 'r' )

    fileData = protocolFile.read()

    protocolFile.close()

    protocolDict = json.loads(fileData)

    fileName = jsonName[ : jsonName.find( '.' ) ]
    fileName = fileName + "Decode.cpp"
    resultFile = open( fileName, 'w' )

    resultFile.write("#include \"ClientProtocolDecode.h\"\n\n")

    resultFile.write("ClientProtocolDecoder::ClientProtocolDecoder()\n")
    resultFile.write("{\n")

    # 디코더 내용 정의
    for k in protocolDict.keys() :
        dictLine = str(protocolDict[k])
        dictLine = dictLine.replace('\'', '\"')
        dictLine = dictLine.replace('None', 'null')
        #print( dictLine )
        protocolTypeDict = json.loads( dictLine )

        resultFile.write("\tdecodeFunction_.insert( std::pair<EnumClientProtocol, DecodeFunction_t>( " + k + ", [] ( PACKET_HEADER*& pck, char* buffer ) -> bool\n")
        resultFile.write("\t{\n")
        resultFile.write("\t\tbuffer += sizeof( PACKET_HEADER );\n\n")

        resultFile.write("\t\tPCK_" + k + "* packet = new (std::nothrow) PCK_" + k + ";\n")

        resultFile.write("\t\tif( packet == nullptr )\n")
        resultFile.write("\t\t\treturn false;\n\n")
	
        for ptdct in protocolTypeDict :
            if type(ptdct['TYPE']) != str :
                continue

            # 배열 처리
            if ptdct['TYPE'].find( '[' ) != -1 :
                arrayType = ptdct['TYPE'][ : ptdct['TYPE'].find( '[' ) ]
                arrayLength = ptdct['TYPE'][ ptdct['TYPE'].find( '[' ) + 1 : ptdct['TYPE'].find( ']' ) ]
                arrayLengthName = "sizeof( " + arrayType + ") * " +  arrayLength;

                resultFile.write( "\t\tmemcpy( &packet->" + ptdct['NAME'] + ", buffer, " + arrayLengthName + " );\n" )
                resultFile.write( "\t\tbuffer += ( "+ arrayLengthName + " );\n" )

            # 벡터
            elif ptdct['TYPE'].find( 'std::vector<' ) != -1 :
                resultFile.write( "\t\tunsigned short count = 0;\n" )
                resultFile.write( "\t\tmemcpy( &count, buffer, sizeof( unsigned short ) );\n" )
                resultFile.write( "\t\tbuffer += sizeof( unsigned short );\n\n" )

                typeName = ptdct['TYPE'][ ptdct['TYPE'].find( '<' ) + 1 : ptdct['TYPE'].find( '>' ) ]
                valueName = ptdct['NAME'][ : -1 ]

                resultFile.write( "\t\tfor( unsigned short i = 0; i < count; ++i )\n" )
                resultFile.write( "\t\t{\n" )
                resultFile.write( "\t\t\t" + typeName + " " + valueName + ";\n" )
                resultFile.write( "\t\t\tmemcpy( &" + valueName + ", buffer, sizeof( " + typeName + " ) );\n" )
                resultFile.write( "\t\t\tbuffer += sizeof( " + typeName + " );\n\n" )

                resultFile.write( "\t\t\tpacket->" + ptdct['NAME'] + ".push_back( " + valueName + " );\n" )
                resultFile.write( "\t\t}\n" )

            # 일반 타입
            else :
                resultFile.write( "\t\tmemcpy( &packet->" + ptdct['NAME'] + ", buffer, sizeof( " + ptdct['TYPE'] +" ) );\n" )
                resultFile.write( "\t\tbuffer += sizeof( "+ ptdct['TYPE'] + " );\n" )

            # 마지막 라인일 경우 줄바꿈 추가
            if ptdct == protocolTypeDict[-1] :
                resultFile.write( "\n" )

        resultFile.write("\t\tpck = packet;\n\n")

        resultFile.write("\t\treturn true;\n")
        resultFile.write("\t} ));\n\n")

    resultFile.write("}\n\n")
    
    resultFile.write("ClientProtocolDecoder::~ClientProtocolDecoder()\n")
    resultFile.write("{\n")
    resultFile.write("\tdecodeFunction_.clear();\n")
    resultFile.write("}\n\n")

    resultFile.write("bool ClientProtocolDecoder::DecodeClientProtocol( EnumClientProtocol protocol, PACKET_HEADER*& pck, char* buffer )\n")
    resultFile.write("{\n")
    resultFile.write("\tif( buffer == nullptr )\n")
    resultFile.write("\t\treturn false;\n\n")

    resultFile.write("\tif( decodeFunction_.find( protocol ) == decodeFunction_.end() )\n")
    resultFile.write("\t\treturn false;\n\n")

    resultFile.write("\treturn decodeFunction_[protocol]( pck, buffer );\n")
    resultFile.write("}\n")

    resultFile.close()

    CopyToClient( fileName )
    
    print("CreateDecoderCppFile End")
#CreateDecoderCppFile end

def CreateDecoderInlineFile( jsonName ) :
    print("CreateDecoderInlineHeader Start")

    protocolFile = open( jsonName, 'r' )

    fileData = protocolFile.read()

    protocolFile.close()

    protocolDict = json.loads(fileData)

    fileName = jsonName[ : jsonName.find( '.' ) ]
    fileName = fileName + "Decode2.h"
    resultFile = open( fileName, 'w' )

    resultFile.write("#pragma once\n")

    resultFile.write("#include \"ClientProtocol.h\"\n\n")

    resultFile.write("#include \"ClientProtocolDecode2.inl\"\n")
    resultFile.close()

    CopyToClient( fileName )

    print("CreateDecoderInlineHeader End")

    # .inl file
    fileName = jsonName[ : jsonName.find( '.' ) ]
    fileName = fileName + "Decode2.inl"
    resultFile = open( fileName, 'w' )

    for k in protocolDict.keys() :
        dictLine = str(protocolDict[k])
        dictLine = dictLine.replace('\'', '\"')
        dictLine = dictLine.replace('None', 'null')
        #print( dictLine )
        protocolTypeDict = json.loads( dictLine )

        resultFile.write("inline bool decode_" + k + "( std::shared_ptr<PACKET_HEADER>& pck, char* buffer )\n")
        resultFile.write("{\n")
        resultFile.write("\tbuffer += sizeof( PACKET_HEADER );\n\n")

        resultFile.write("\tstd::shared_ptr<PCK_" + k + "> packet = std::make_shared<PCK_" + k + ">();\n")

        resultFile.write("\tif( packet == nullptr )\n")
        resultFile.write("\t\treturn false;\n\n")
	
        for ptdct in protocolTypeDict :
            if type(ptdct['TYPE']) != str :
                continue

            # 배열 처리
            if ptdct['TYPE'].find( '[' ) != -1 :
                arrayType = ptdct['TYPE'][ : ptdct['TYPE'].find( '[' ) ]
                arrayLength = ptdct['TYPE'][ ptdct['TYPE'].find( '[' ) + 1 : ptdct['TYPE'].find( ']' ) ]
                arrayLengthName = "sizeof( " + arrayType + ") * " +  arrayLength;

                resultFile.write( "\tmemcpy( &packet->" + ptdct['NAME'] + ", buffer, " + arrayLengthName + " );\n" )
                resultFile.write( "\tbuffer += ( "+ arrayLengthName + " );\n" )

            # 벡터
            elif ptdct['TYPE'].find( 'std::vector<' ) != -1 :
                resultFile.write( "\tunsigned short count = 0;\n" )
                resultFile.write( "\tmemcpy( &count, buffer, sizeof( unsigned short ) );\n" )
                resultFile.write( "\tbuffer += sizeof( unsigned short );\n\n" )

                typeName = ptdct['TYPE'][ ptdct['TYPE'].find( '<' ) + 1 : ptdct['TYPE'].find( '>' ) ]
                valueName = ptdct['NAME'][ : -1 ]

                resultFile.write( "\tfor( unsigned short i = 0; i < count; ++i )\n" )
                resultFile.write( "\t{\n" )
                resultFile.write( "\t\t" + typeName + " " + valueName + ";\n" )
                resultFile.write( "\t\tmemcpy( &" + valueName + ", buffer, sizeof( " + typeName + " ) );\n" )
                resultFile.write( "\t\tbuffer += sizeof( " + typeName + " );\n\n" )

                resultFile.write( "\t\tpacket->" + ptdct['NAME'] + ".push_back( " + valueName + " );\n" )
                resultFile.write( "\t}\n" )

            # 일반 타입
            else :
                resultFile.write( "\tmemcpy( &packet->" + ptdct['NAME'] + ", buffer, sizeof( " + ptdct['TYPE'] +" ) );\n" )
                resultFile.write( "\tbuffer += sizeof( "+ ptdct['TYPE'] + " );\n" )

            # 마지막 라인일 경우 줄바꿈 추가
            if ptdct == protocolTypeDict[-1] :
                resultFile.write( "\n" )

        resultFile.write("\tpck = std::static_pointer_cast<PACKET_HEADER>( packet );\n\n")
        resultFile.write("\treturn true;\n")
        resultFile.write("};\n\n")

    resultFile.write("inline bool DecodeClientProtocol(EnumClientProtocol protocol, std::shared_ptr<PACKET_HEADER>& pck, char* buffer)\n")
    resultFile.write("{\n")
    resultFile.write("\tswitch( protocol )\n")
    resultFile.write("\t{\n")

    for k in protocolDict.keys() :
        resultFile.write("\tcase " + k + " :\n")
        resultFile.write("\t\treturn decode_" + k + "( pck, buffer );\n")

    resultFile.write("\tdefault :\n")
    resultFile.write("\t\tbreak;\n")

    resultFile.write("\t}\n\n")
    resultFile.write("\t\treturn false;\n\n")
    resultFile.write("}\n")

    resultFile.close()

    CopyToClient( fileName )
#CreateDecoderInlineFile end

#Copy define files
CopyToServer( "ProtocolDefine.h" )
CopyToClient( "ProtocolDefine.h" )

CreateProtocolFile( "ClientProtocol.json" )
CreateEncoderFile( "ClientProtocol.json" )
CreateEncoderInlFile( "ClientProtocol.json" )
CreateDecoderFile( "ClientProtocol.json" )
CreateDecoderCppFile( "ClientProtocol.json" )
CreateDecoderInlineFile( "ClientProtocol.json" )



