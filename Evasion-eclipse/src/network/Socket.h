/* 
   Template by René Nyffenegger (rene.nyffenegger@adp-gmbh.ch, http://www.adp-gmbh.ch/win/misc/sockets.html)
   
   I renamed a lot of the variables and used exceptions and made some other little changes.
*/


#ifndef SOCKET_H
#define SOCKET_H

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

#include <WinSock2.h>
#include "NetworkException.h"

enum SocketType {BlockingSocket, NonBlockingSocket};

class Socket {
	public:
		virtual ~Socket();
		Socket(const Socket&);
		Socket& operator=(Socket&);

		std::string receiveLine();
		std::string receiveBytes();

		void close();

		// The parameter of SendLine is not a const reference
		// because SendLine modifes the std::string passed.
		void sendLine(std::string);

		// The parameter of SendBytes is a const reference
		// because SendBytes does not modify the std::string passed 
		// (in contrast to SendLine).
		void sendBytes(const std::string&);

	protected:
		friend class ServerSocket;
		friend class SelectionSocket;

		Socket(SOCKET s);
		Socket();


		SOCKET s_;

		int* refCounter_;
	private:
		static void start();
		static void end();
		static int  socketCount;
};

#endif
