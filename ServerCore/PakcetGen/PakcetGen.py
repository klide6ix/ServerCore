import json
from os import path
import os
import shutil

EncodeList = [ "..\TestServer", "..\TestClient" ]
DecodeList = [ "..\TestServer", "..\TestClient" ]

def CopyToEncode( FileName ) :
	for AbsPath in EncodeList :
		EncodePath = os.path.abspath( AbsPath )
		dstFile = os.path.join( EncodePath, FileName )
		#print( dstFile )

		srcFile = os.path.join( os.path.abspath( ".\\" ), FileName )
		#print( srcFile )

		if os.path.exists(dstFile):
			os.remove(dstFile)
		shutil.copy( srcFile, dstFile )

def CopyToDecode( FileName ) :
	for AbsPath in DecodeList :
		DecodePath = os.path.abspath( AbsPath )
		dstFile = os.path.join( DecodePath, FileName )
		#print( dstFile )

		srcFile = os.path.join( os.path.abspath( ".\\" ), FileName )
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

	orgFileName = jsonName[ : jsonName.find( '.' ) ]
	fileName = orgFileName + ".h"
	
	# 프로토콜 버전
	PROTOCOL_VERSION = 1000
	if os.path.exists(fileName):
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
	resultFile.write("enum Enum" + orgFileName + "\n")
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

	CopyToEncode( fileName )
	CopyToDecode( fileName )

	print("CreateProtocolFile End")
#CreateProtocolFile end

def ParseFunctionLine( resultFile, protocolName, dictLine, isDefine, isEncode ) :
	dictLine = dictLine.replace('\'', '\"')
	dictLine = dictLine.replace('None', 'null')
	#print(dictLine)
	protocolTypeDict = json.loads( dictLine )

	#print( protocolName, protocolTypeDict )

	resultFile.write( "inline static bool ")
	if isEncode == True :
		resultFile.write( "encode_" )
	else :
		resultFile.write( "decode_" )

	resultFile.write( protocolName + "( BufferSerializer& serializer" )

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
			if isEncode == True :
				resultFile.write( " const" )
			resultFile.write( " " + arrayType + "* " + ptdct['NAME'] + ",")
			if isEncode == True :
				resultFile.write( " const" )
			resultFile.write( " unsigned short& " + arrayLengthName )

		# 벡터
		elif ptdct['TYPE'].find( 'std::vector<' ) != -1 :
			resultFile.write( " IEncodeIterator* " + ptdct['NAME'] )

		# 일반 타입
		else :
			if isEncode == True :
				resultFile.write( " const" )
			resultFile.write( " " + ptdct['TYPE'] + "& " + ptdct['NAME'] )

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

	orgFileName = jsonName[ : jsonName.find( '.' ) ]
	fileName = orgFileName + "Encode.h"
	resultFile = open( fileName, 'w' )

	# 설정
	resultFile.write("#pragma once\n\n")
	resultFile.write("#include \"" + orgFileName + ".h\"\n\n")
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
		ParseFunctionLine( resultFile, k, str(protocolDict[k]), True, True )

	resultFile.write("\n#include \"" + orgFileName + "Encode.inl\"\n")
	resultFile.close()

	CopyToEncode( fileName )

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
		ParseFunctionLine( resultFile, k, str(protocolDict[k]), False, True )

		dictLine = str(protocolDict[k])
		dictLine = dictLine.replace('\'', '\"')
		dictLine = dictLine.replace('None', 'null')
		#print( dictLine )
		protocolTypeDict = json.loads( dictLine )

		# 내용
		resultFile.write("{\n")
		resultFile.write("\tPACKET_HEADER* header = serializer.GetTypePointer<PACKET_HEADER>();\n")
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
				resultFile.write( "\tunsigned short* count = serializer.GetTypePointer<unsigned short>();\n" )

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
	
		resultFile.write("\theader->size_ = static_cast<unsigned short>( serializer.GetSize() );\n")

		resultFile.write("\n\treturn true;\n")
		resultFile.write("}\n")

	resultFile.close()

	CopyToEncode( fileName )

	print("CreateEncoderInlFile End")
#CreateEncoderInlFile end

def CreateDecoderFile( jsonName ) :
	print("CreateDecoderFile Start")

	protocolFile = open( jsonName, 'r' )

	fileData = protocolFile.read()

	protocolFile.close()

	protocolDict = json.loads(fileData)

	orgFileName = jsonName[ : jsonName.find( '.' ) ]
	fileName = orgFileName + "Decode.h"
	resultFile = open( fileName, 'w' )

	resultFile.write("#pragma once\n\n")
	resultFile.write("#include \"" + orgFileName + ".h\"\n\n")
	resultFile.write("#include \"..\\Utility\\BufferSerializer.h\"\n\n")

	for k in protocolDict.keys() :
		resultFile.write("inline static bool decode_" + k + "( BufferSerializer& serializer, PCK_" + k + "& pck );\n")

	resultFile.write("\n#include \"" + orgFileName + "Decode.inl\"\n")
	resultFile.close()

	CopyToDecode( fileName )
	
	print("CreateDecoderFile End")
#CreateDecoderFile end

def CreateDecoderInlineFile( jsonName ) :
	print("CreateDecoderInlineHeader Start")

	protocolFile = open( jsonName, 'r' )

	fileData = protocolFile.read()

	protocolFile.close()

	protocolDict = json.loads(fileData)

	orgFileName = jsonName[ : jsonName.find( '.' ) ]
	fileName = orgFileName + "Decode.inl"
	resultFile = open( fileName, 'w' )

	for k in protocolDict.keys() :

		resultFile.write("inline static bool decode_" + k + "( BufferSerializer& serializer, PCK_" + k + "& pck )\n")

		dictLine = str(protocolDict[k])
		dictLine = dictLine.replace('\'', '\"')
		dictLine = dictLine.replace('None', 'null')
		#print( dictLine )
		protocolTypeDict = json.loads( dictLine )

		# 내용
		resultFile.write("{\n")
		resultFile.write("\tPACKET_HEADER* header = serializer.GetTypePointer<PACKET_HEADER>();\n")
		resultFile.write("\tif( header->protocol_ != " + k + " )\n")
		resultFile.write("\t\treturn false;\n\n")

		for ptdct in protocolTypeDict :
			if type(ptdct['TYPE']) != str :
				continue

			# 배열 처리
			if ptdct['TYPE'].find( '[' ) != -1 :
				resultFile.write( "\tunsigned short size = 0;\n" )
				resultFile.write( "\tserializer.get_data( pck." + ptdct['NAME'] + ", size );\n" );

			# 벡터
			elif ptdct['TYPE'].find( 'std::vector<' ) != -1 :

				vectorType = ptdct['TYPE'][ ptdct['TYPE'].find( '<' ) + 1 : ptdct['TYPE'].find( ']' ) ]
				resultFile.write( "\tunsigned short* count = serializer.GetTypePointer<unsigned short>();\n" )

				resultFile.write( "\tif( count == nullptr )\n" )
				resultFile.write( "\t\treturn false;\n\n" )

				resultFile.write( "\tfor( unsigned short i = 0; i < (*count); ++i )\n" )
				resultFile.write( "\t{\n" )
				resultFile.write( "\t\t" + vectorType + " temp;\n" )
				resultFile.write( "\t\tserializer.get_data( temp );\n" )
				resultFile.write( "\t\tpck." + ptdct['NAME'] + ".push_back( temp );\n" )
				resultFile.write( "\t}\n" )

			# 일반 타입
			else :
				resultFile.write( "\tserializer.get_data( pck." + ptdct['NAME'] + " );\n" )

			# 마지막 라인일 경우 줄바꿈 추가
			if ptdct == protocolTypeDict[-1] :
				resultFile.write( "\n" )
	
		resultFile.write("\n\treturn true;\n")
		resultFile.write("}\n")

	resultFile.close()

	CopyToDecode( fileName )
#CreateDecoderInlineFile end

#Copy define files
CopyToEncode( "ProtocolDefine.h" )
CopyToDecode( "ProtocolDefine.h" )

CreateProtocolFile( "TestProtocol.json" )

CreateEncoderFile( "TestProtocol.json" )
CreateEncoderInlFile( "TestProtocol.json" )

CreateDecoderFile( "TestProtocol.json" )
CreateDecoderInlineFile( "TestProtocol.json" )



