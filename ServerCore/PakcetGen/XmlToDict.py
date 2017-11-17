from xml.etree.ElementTree import parse


def ReadFileXml( inputFileName ) :
	tree = parse( inputFileName )
	protocolAll = tree.getroot()
	
	protocolDict = {}

	for protocol in protocolAll :
		protocolName = protocol.attrib["NAME"]
		protocolDict[protocolName] = []
		for protocolElem in protocol :
			protocolDict[protocolName].append( dict(protocolElem.items()) )

	return protocolDict


#ReadFileXml( "TestProtocol.xml" )