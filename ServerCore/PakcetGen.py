import json
 
protocolFile = open( "ClientProtocol.json", 'r' )

fileData = protocolFile.read()
#print(fileData)

protocolFile.close()

protocolDict = json.loads(fileData)
#print(protocolDict)

resultFile = open( "ClientProtocol.h", 'w' )

# 프로토콜 Enum
resultFile.write("enum EnumClientProtocol\n")
resultFile.write("{\n")

for k in protocolDict.keys() :
    
    dictLine = str(protocolDict[k])
    dictLine = dictLine.replace('\'', '\"')
    dictLine = dictLine.replace('None', 'null')
    #print( dictLine )
    protocolTypeDict = json.loads( dictLine )
    
    for ptdct in protocolTypeDict:
        if 'INDEX' in ptdct :
            linedata = "\t" + k + " = " + ptdct['INDEX'] + ",\n"
        else :
            linedata = "\t" + k + ",\n"

    resultFile.write(linedata)
resultFile.write("};\n\n")

# 구조체
for k in protocolDict.keys() :
    dictLine = str(protocolDict[k])
    dictLine = dictLine.replace('\'', '\"')
    dictLine = dictLine.replace('None', 'null')
    protocolTypeDict = json.loads( dictLine )

    resultFile.write("struct PCK_" + k + " : public _PACKET_HEADER\n")
    resultFile.write("{\n")

    # 멤버 변수
    for ptdct in protocolTypeDict:
        if 'INDEX' in ptdct :
            linedata = "\t" + k + " = " + ptdct['INDEX'] + ",\n"
        else :
            linedata = "\t" + k + ",\n"

    resultFile.write(linedata)

    # 생성자
    resultFile.write("\tPCK_" + k + "() : _PACKET_HEADER()\n")
    resultFile.write("\t{\n")
    resultFile.write("\t}\n")
    resultFile.write("};\n\n")

resultFile.close()
