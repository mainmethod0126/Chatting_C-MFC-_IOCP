#pragma once
#include <vector>
#include <string>
#include <mswsock.h>
#include "WS2tcpip.h"

class ClientInfo
{
private:
	int m_nUserKey;
	SOCKET m_Socket;
	
};

