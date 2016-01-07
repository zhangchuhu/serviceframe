#include "UdpConnImp.h"
#include "core/sox/logger.h"
using namespace core;
using namespace sox;

UdpConnImp::UdpConnImp(IConn *s, int peerip, int pport, ILinkHandler *h, ILinkEvent *eh){
	serverConn = NULL;
	reset(peerip, pport);
	//not bind
	setHandler(h);
	setLinkEvent(eh);
}

UdpConnImp::UdpConnImp(){
	serverConn = NULL;
	reset(0, 0);
	//not bind
	setHandler(NULL);
	setLinkEvent(NULL);
}

void UdpConnImp::reset(int ip, int port){
	peerIp = ip;
	peerPort = port;
	
	memset(&sa, 0, sizeof(sa));

	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = ip;
	sa.sin_port = htons((u_short)port);
}
/*
UdpConnImp::UdpConnImp(const std::string &ip, int sport, ILinkHandler *h) :
	Udpsock(ip, sport) {
	u_long sip =:: inet_addr(ip.c_str());

	memset(&sa, 0, sizeof(sa));

	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = sip;
	sa.sin_port = htons((u_short)sport);
	
	peerIp = sip;
	peerPort = sport;

	setHandler(h);

}*/

void UdpConnImp::setServer(UdpServerImp *ser){
	serverConn = ser;
}

void UdpConnImp::sendBin(const char *data, size_t sz, uint32_t uri) {
	serverConn->sendto(data, sz, &sa, sizeof(sa));
}


UdpServerImp::UdpServerImp(int localPort, ILinkHandler *h, ILinkEvent *eh):
Udpsock(localPort) {
	setHandler(h);
	setLinkEvent(eh);
	select(0, SEL_READ);
}

void UdpServerImp::onRead() {
	socklen_t solen_dummy = sizeof(sox::ipaddr_type);

	ipaddr_type addr;

	memset(&addr, 0, sizeof(addr));

	int m_size = recvfrom(m_buffer, sizeof(m_buffer), &addr, &solen_dummy);

	u_long ip = addr.sin_addr.s_addr;
	int port = ntohs(addr.sin_port);

	
	UdpConnImp *p = (UdpConnImp *)connManager->createServerConn((SOCKET)0, ip, port, dHandler, eHandler);
	
	p->setServer(this);
	try{
		dHandler->onData(m_buffer, m_size, p, UDP_UNCODE);
	}catch(std::exception & ex){
		log(Error, "handle proc error:%s", ex.what());
		eHandler->onError(Error, ex.what(), p);
	}
}
