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

#include "SelectionSocket.h"

SelectionSocket::SelectionSocket(Socket const * const socket1, Socket const * const socket2, SocketType type) {
	FD_ZERO(&fds_);
	FD_SET(const_cast<Socket*>(socket1)->s_, &fds_);
	if(socket2) {
		FD_SET(const_cast<Socket*>(socket2)->s_, &fds_);
	}     

	TIMEVAL tval;
	tval.tv_sec  = 0;
	tval.tv_usec = 1;

	TIMEVAL *ptval;
	if(type==NonBlockingSocket) {
		ptval = &tval;
	} else { 
		ptval = 0;
	}

	if (select (0, &fds_, (fd_set*) 0, (fd_set*) 0, ptval) == SOCKET_ERROR) {
		throw NetworkException(std::string("Error in select"));
	}
}

bool SelectionSocket::isReadable(Socket const* const socket) {
	if (FD_ISSET(socket->s_,&fds_)) { 
		return true;
	}
	return false;
}
