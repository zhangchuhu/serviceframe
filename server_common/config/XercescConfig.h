#include _XERCESCCONFIG_H_FXW_
#define _XERCESCCONFIG_H_FXW_
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>

namespace server{
	namespace config{
		class XercescConfig{
		public:
			XercescConfig();
			~XercescConfig();

			void setFileName();
			void load();

			static string toString(XmlString *);
			XercesDOMParser *getDomParse();
		private:
			XercesDOMParser *m_ConfigFileParser;
		};
	}
}
#endif