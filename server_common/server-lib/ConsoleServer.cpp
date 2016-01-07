#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#include "server_common/helper/main_inc.h"
#include "server_common/server-lib/ConsoleServer.h"
#include "core/sox/logger.h"
#include "core/sox/measure.h"

using namespace core;
using namespace std;

ConsoleServer::ConsoleServer(IConnManager *conM, IDaemonServerAware *ds, string serviceName, uint16_t port,
	bool statFrom, bool statByIp)
{
    m_startTime = sox::env::now;
	m_serviceName = serviceName;
	m_daemonServer = ds;
	m_bStatByIp = statByIp;
	m_bStatFrom = statFrom;

	//Start net server
    
    vector<uint16_t> ports;
    ports.push_back(port);
    outerServer.refreshPorts(ports);	
    outerServer.setConnManager(conM);
    outerServer.setLinkHandler(this);
    outerServer.setLinkEvent(this);	

	string help;
	help += "agent get proc\n";
	help += "agent get stat\n";
	help += "agent get dep\n";
	help += "agent get all\n";
	help += "agent get clear\n";
	help += "agent get value testValue\n";
	help += "agent set testValue=xxx\n";
	addEntry("agent", help, (ConsoleServerAware*)this, (consoleCall)(&ConsoleServer::cmdAgent));
	
	addEntry("help", "This help list\n", (ConsoleServerAware*)this, (consoleCall)(&ConsoleServer::cmdHelp));

	start();
	
}

ConsoleServer::~ConsoleServer()
{
}

void ConsoleServer::start()
{
	outerServer.startSV();
	std::vector<uint16_t> realPorts = outerServer.getPorts();
	uint16_t realPort = realPorts.front();
	
	//touch /home/dspeak/yyms/proc_info/pid-starttime-port
	//
	mkdir("/home/dspeak/yyms", 777);
	mkdir("/home/dspeak/yyms/proc_info", 777);
	string filename = "/home/dspeak/yyms/proc_info/";
	filename += int2string(getpid()) + "-" + int2string(m_startTime) + "-" + int2string(realPort);
	FILE *fd = fopen(filename.c_str(), "a+");
	if(fd == NULL){
		printf("yyms generate file: %s fail.", filename.c_str());
		exit(0);
	}
	fclose(fd);
}

int ConsoleServer::onData(const char* data, size_t len, IConn *conn, int type)
{
    if(len > getPackLimit())
        return -1;

    std::string input_data(data, len);
    std::string line;
    string_util::getline(input_data, line);
    if(line.size()) {
        process_command(line, conn);
    }
    return len;
}

void ConsoleServer::process_command(std::string cmd, core::IConn* conn)
{
	std::string ip = sox::addr_ntoa(conn->getPeerIp());
    log(Info, "ConsoleCommand: %s, from %s", cmd.data(), ip.c_str());

	// if enter is "Up key", proccess the last command.
	const char lastCmd[4]={0x1b,0x5b,0x41,0x00};
	if(cmd.size() == 3 && memcmp(cmd.c_str(), lastCmd, 3) == 0){
		cmd = m_lastCmd;
    }
	m_lastCmd = cmd;

    uint32_t npos = cmd.find_first_of(" ");
    std::string cmdstr;
    std::string params;
    if(npos == std::string::npos){
        cmdstr = cmd;
    } else {
        cmdstr = cmd.substr(0, npos);
        params = cmd.substr(npos+1);
    }
	
	string response;
	map<string, consoleCallback>::iterator it = m_cmdMap.find(cmdstr);
	if(it != m_cmdMap.end()){
		struct consoleCallback &cc = it->second;
		response = (cc.ica->*cc.icc)(params, conn);
	}else if(sox::addr_ntoa(conn->getPeerIp()) == "127.0.0.1") {
		response = "Unknown cmd, use help to look up support cmd.\n";
	}
	
    conn->sendBin(response.data(), response.size(), 0);	
}

string ConsoleServer::cmdHelp(std::string params, core::IConn* conn)
{	
    if(sox::addr_ntoa(conn->getPeerIp()) != "127.0.0.1") {
		return "";
    }
    std::string response("Support command:\n");
	for(map<string, consoleCallback>::iterator it = m_cmdMap.begin(); it != m_cmdMap.end(); ++it){
		struct consoleCallback &cc = it->second;
		response += it->first + " : \n" + cc.help + "\n";
	}
    
    return response;
}



string ConsoleServer::cmdAgent(std::string params, core::IConn* conn){
	
	std::string response;
	if(params == "get stat"){
		response ="{\n" +  measureDumpJSON() + "\n}\n\r\n\r\n";
	}else if(params == "get proc"){
		response = "{\n" + getProcInfo2JSON() + "\n}\n\r\n\r\n";
	}else if(params == "get dep"){
		response = "{\n" + resourceStat2JSON() + "\n}\n\r\n\r\n";
	}else if(params == "get all"){
		response = "{\n" + getProcInfo2JSON() + ",\n" + measureDumpJSON(m_bStatFrom, m_bStatByIp, false) + ",\n" + resourceStat2JSON() + "\n}\n\r\n\r\n";
	}else if(params == "get clear"){
		response = "{\n" + getProcInfo2JSON() + ",\n" + measureDumpJSON(m_bStatFrom, m_bStatByIp, true) + ",\n" + resourceStat2JSON() + "\n}\n\r\n\r\n";
/*	}else if(params.substr(0, 3) == "set"){
		uint32_t epos = params.find_first_of("=");
		if(epos == std::string::npos){
			response = "wrong format, usage: agent set key=value\n";
		}else{
			std::string key = params.substr(4, epos - 4);
			std::string value = params.substr(epos+1);
			response = "\"key\":" + key + " = " + value + "\n\r\n\r\n";
			if(key == "testValue"){
				m_testValue = atoi(value.c_str());
			}else{
				response += "key not found\n\r\n\r\n";
			}
		}
	}else if(params.substr(0, 9) == "get value"){
		std::string key = params.substr(10);
		response = "\"key\":" + key;
		if(key == "testValue"){
			response += " = " + string(int2str(m_testValue)) + "\n\r\n\r\n";
		}else{
			response += " not found\n\r\n\r";
		}
		*/
	}else{
		response = "unknown command.\n\r\n\r";
	}
	
	return response;
}


string ConsoleServer::getProcInfo2JSON()
{
	std::ostringstream os;	
	os << "	\"proc\":" << std::endl;
	os << "	{" << std::endl;
	os << "		\"pname\":\"" << m_serviceName << "\"," << std::endl;
	os << "		\"pid\":" << int2str(getpid()) << "," << std::endl;
	os << "		\"serverId\":" << int2str(m_daemonServer->getDaemonServer()->getServerId()) << "," << std::endl;
	os << "		\"pver\":" << int2str(SERVER_VERSION) << "," << std::endl;
	for(std::map<std::string, procInfoCallback>::iterator it = m_procInfoMap.begin(); it != m_procInfoMap.end(); ++it){
		procInfoCallback &pc = it->second;
		os << " 		\"" << it->first <<"\":" << (pc.ica->*pc.icc)() << "," << std::endl;
	}
	os << "		\"time\":" << int2str(m_startTime) << std::endl;
	os << "	}";
	return os.str();
}

string ConsoleServer::resourceStat2JSON()
{	
	bool firstOutputElement = true;
	std::ostringstream os;	
	os << "	\"dep\":" << std::endl;
	os << "	[" ;
	for(std::map<string, resourceStat*>::iterator it = m_resourceStat.begin(); it != m_resourceStat.end(); ++it){
		if(firstOutputElement == false){
			os << "," << std::endl;
		}else{
			firstOutputElement = false;
			os << std::endl;
		}
		const string &resourceName = it->first;
		struct resourceStat *rst = it->second;
		os << "		{\"name\":\"" << resourceName << "\"," << std::endl;
		os << "		\"sended\":" << int2str(rst->requestCount) << "," << std::endl;
		os << "		\"lost\":" << int2str(rst->lostCount) << std::endl;
		os << "		}" ;
	}
	os << std::endl << "	]" << std::endl;
	return os.str();
}

void ConsoleServer::addEntry(std::string cmd, std::string help, ConsoleServerAware *ica, consoleCall icc)
{
	consoleCallback &cm = m_cmdMap[cmd];
	cm.help = help;
	cm.ica = ica;
	cm.icc = icc;
}
void ConsoleServer::addProcInfo(std::string infoName, ConsoleServerAware *ica, procInfoCall icc)
{
	procInfoCallback &pc = m_procInfoMap[infoName];
	pc.name = infoName;
	pc.ica = ica;
	pc.icc = icc;
}

void ConsoleServer::addResourceStat(std::string resourceName, resourceStat* rs)
{
	m_resourceStat[resourceName] = rs;
}

console_handler* ConsoleServer::create(SOCKET so, uint32_t ip, int port)
{
    console_handler* handler = new console_handler(so, ip, port, this, this);
    handler->select(0, sox::SEL_READ);
    __conns.insert(handler);
    return handler;
}
void ConsoleServer::destroy(console_handler* handler)
{
    delete handler;
    __conns.erase(handler);
}

void ConsoleServer::onAccept(SOCKET so, u_long ip, int port)
{
    create(so, ip, port);
}

void ConsoleServer::onClose(IConn *conn)
{
    destroy((console_handler*)conn);
}

void ConsoleServer::onError(int ev, const char *msg, IConn *conn)
{
    onClose(conn);
}



