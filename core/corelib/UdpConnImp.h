#ifndef UDPCONNIMP_H_
#define UDPCONNIMP_H_

#include "core/sox/udpsock.h"
#include "common/core/iserver.h"
#include "AbstractConn.h"

namespace core
{
class UdpServerImp;
class UdpConnImp : public AbstractConn
{
	sox::ipaddr_type sa;

	UdpServerImp *serverConn;
public:
	//connect
	UdpConnImp(IConn * server, int peerIp, int pport, ILinkHandler *h, ILinkEvent *eh);
	UdpConnImp();
	//client
	//UdpConnImp(const std::string &sip, int sport, ILinkHandler *h);
	void reset(int peerIp, int port);
	//client,  connect
	virtual void sendBin(const char *data, size_t sz, uint32_t uri);

	virtual void setEncKey(const unsigned char*, size_t){}
	 
	virtual void setTimeout(int){}

	void setServer(UdpServerImp *ser);
};

class UdpServerImp: public sox::Udpsock, public AbstractConn, public IConnManagerAware{
	//xxx TODO size
	char m_buffer[1024];
public:	
	//server
	UdpServerImp(int localPort, ILinkHandler *h, ILinkEvent *eh);

	virtual void sendBin(const char *data, size_t sz, uint32_t uri){
		assert(false);
	}
	virtual void setEncKey(const unsigned char*, size_t){}
	//server
	virtual void onRead();

	virtual void setTimeout(int){}
};
}

#endif /*UDPCONNIMP_H_*/
