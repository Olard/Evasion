/* 
   Template by René Nyffenegger (rene.nyffenegger@adp-gmbh.ch, http://www.adp-gmbh.ch/win/misc/sockets.html)
   
   I renamed a lot of the variables and used exceptions and made some other little changes.
*/

/*
    Copyright (C) 2002-2004 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/

#include "ServerSocket.h"

Socket* ServerSocket::acceptClient() {
  SOCKET new_sock = accept(s_, 0, 0);
  if (new_sock == INVALID_SOCKET) {
    int rc = WSAGetLastError();
    if(rc==WSAEWOULDBLOCK) {
      return 0; // non-blocking call, no request pending
    }
    else {
      throw NetworkException(std::string("Invalid Socket"));
    }
  }

  Socket* r = new Socket(new_sock);
  return r;
}

ServerSocket::ServerSocket(int port, int connections, SocketType type) {
	sockaddr_in sa;

	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;             
	sa.sin_port = htons(port);          
	s_ = socket(AF_INET, SOCK_STREAM, 0);
	if (s_ == INVALID_SOCKET) {
		throw NetworkException(std::string("INVALID_SOCKET"));
	}

	if(type==NonBlockingSocket) {
		u_long arg = 1;
		ioctlsocket(s_, FIONBIO, &arg);
	}

	/* bind the socket to the internet address */
	if (bind(s_, (sockaddr *)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR) {
		closesocket(s_);
		throw NetworkException(std::string("INVALID_SOCKET"));
	}

	listen(s_, connections);                               
}

