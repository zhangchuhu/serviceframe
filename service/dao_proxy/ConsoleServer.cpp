#include <string>

#include "server_common/helper/main_inc.h"
#include "ConsoleServer.h"
#include "core/sox/measure.h"

using namespace server;
using namespace server::cache;
using namespace core;
using namespace std;

std::vector<std::string> tokenize_str(const std::string & str,
									  const std::string & delims=", \t")
{
	using namespace std;
	// Skip delims at beginning, find start of first token
	string::size_type lastPos = str.find_first_not_of(delims, 0);
	// Find next delimiter @ end of token
	string::size_type pos     = str.find_first_of(delims, lastPos);

	// output vector
	vector<string> tokens;

	while (string::npos != pos || string::npos != lastPos)
	{
		// Found a token, add it to the vector.
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		// Skip delims.  Note the "not_of". this is beginning of token
		lastPos = str.find_first_not_of(delims, pos);
		// Find next delimiter at end of token.
		pos     = str.find_first_of(delims, lastPos);
	}

	return tokens;
}

ConsoleServer::ConsoleServer()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	m_startTime = tv.tv_sec;
}

ConsoleServer::~ConsoleServer()
{
}

console_handler* ConsoleServer::create(SOCKET so, u_long ip, int port)
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

int ConsoleServer::onData(const char* data, size_t len, IConn *conn, int type)
{
	if(len > getPackLimit())
		return -1;

	std::string input_data(data, len);

	std::string line;
	string_util::getline(input_data, line);
	if(line.size())
	{
		process_command(line, conn);
	}

	return len;
}

void ConsoleServer::onClose(IConn *conn)
{
	destroy((console_handler*)conn);
}

void ConsoleServer::onError(int ev, const char *msg, IConn *conn)
{
	onClose(conn);
}

string ConsoleServer::getProcInfo2JSON()
{
	std::ostringstream os;	
	os << "	\"proc\":" << std::endl;
	os << "	{" << std::endl;
	os << "		\"pname\":\"" << m_server->getName() << "\"," << std::endl;
	os << "		\"pid\":\"" << getpid() << "\"," << std::endl;
	os << "		\"serverId\":" << int2str(m_server->getServerId()) << "," << std::endl;
	os << "		\"groupId\":" << int2str(m_server->getGroupId()) << "," << std::endl;
	os << "		\"pver\":" << int2str(SERVER_VERSION) << "," << std::endl;
	os << "		\"time\":" << int2str(m_startTime) << std::endl;
	os << "	}";
	return os.str();
}
void ConsoleServer::process_command(std::string cmd, core::IConn* conn)
{
	std::string ip = sox::addr_ntoa(conn->getPeerIp());
	log(Info, "ConsoleCommand: %s, from %s", cmd.data(), ip.c_str());

	const char lastCmd[4]={0x1b,0x5b,0x41,0x00};
	if(cmd.size() == 3 && memcmp(cmd.c_str(), lastCmd, 3) == 0){
		cmd = m_lastCmd;
	}
	m_lastCmd = cmd;

	uint32_t npos = cmd.find_first_of(" ");

	std::string cmdstr;
	std::string params;

	if(npos == std::string::npos)
	{
		cmdstr = cmd;
	} 
	else 
	{
		cmdstr = cmd.substr(0, npos);
		params = cmd.substr(npos+1);
	}

	if(cmdstr == "help" && ip == "127.0.0.1")
	{
		std::string response("Support command:\n");
		response += "agent get proc\n";
		response += "agent get stat\n";
		response += "agent get all\n";
		response += "agent get clear\n";

		conn->sendBin(response.data(), response.size(), 0);	
		return;
	}

	if (cmdstr == "agent")
	{
		std::string response;
		if(params == "get stat"){
			response ="{\n" +  measureDumpJSON() + "\n}\n\r\n\r\n";
		}else if(params == "get proc"){
			response = "{\n" + getProcInfo2JSON() + "\n}\n\r\n\r\n";
		}else if(params == "get all"){
			response = "{\n" + getProcInfo2JSON() + ",\n" + measureDumpJSON(true, true, false) + "\n}\n\r\n\r\n";
		}else if(params == "get clear"){
			response = "{\n" + getProcInfo2JSON() + ",\n" + measureDumpJSON(true, true, true) + "\n}\n\r\n\r\n";
		}else{
			response = "unknown command.\n\r\n\r\n";
		}
		conn->sendBin(response.data(), response.size(), 0);	
		return;
	}
}

void ConsoleServer::onAccept(SOCKET so, u_long ip, int port)
{
	create(so, ip, port);
}
void ConsoleServer::setDaemonServer(core::IDaemonServer *server)
{
	m_server = server;
}
