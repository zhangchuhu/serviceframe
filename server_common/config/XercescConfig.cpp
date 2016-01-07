#include "XercescConfig.h"
using namespace server::config;

XercescConfig::XercescConfig(){
	try
	{
		XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
	}
	catch( XMLException& e )
	{
		char* message = XMLString::transcode( e.getMessage() );
		cerr << "XML toolkit initialization error: " << message << endl;
		XMLString::release( &message );
		// throw exception here to return ERROR_XERCES_INIT
	}

	// Tags and attributes used in XML file.
	// Can't call transcode till after Xerces Initialize()
	m_ConfigFileParser = new XercesDOMParser;
}

XercescConfig::~XercescConfig(){
	try
	{
		XMLPlatformUtils::Terminate();  // Terminate Xerces
	}
	catch( xercesc::XMLException& e )
	{
		char* message = xercesc::XMLString::transcode( e.getMessage() );

		cerr << "XML ttolkit teardown error: " << message << endl;
		XMLString::release( &message );
	}
}

XercesDOMParser *XercescConfig::getDomParse(){
	return m_ConfigFileParser;
}