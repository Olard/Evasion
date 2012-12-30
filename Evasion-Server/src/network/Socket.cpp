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

#include "Socket.h"
#include <iostream>

using namespace std;

int Socket::socketCount= 0;

void Socket::start() {
  if (!socketCount) {
    WSADATA info;
    if (WSAStartup(MAKEWORD(2,0), &info)) {
      throw NetworkException(std::string("Could not start WSA"));
    }
  }
  ++socketCount;
}

void Socket::end() {
  WSACleanup();
}

Socket::Socket() : s_(0) {
  start();
  // UDP: use SOCK_DGRAM instead of SOCK_STREAM
  s_ = socket(AF_INET,SOCK_STREAM,0);

  if (s_ == INVALID_SOCKET) {
    throw NetworkException(std::string("INVALID_SOCKET"));
  }

  refCounter_ = new int(1);
}

Socket::Socket(SOCKET s) : s_(s) {
  start();
  refCounter_ = new int(1);
};

Socket::~Socket() {
  if (! --(*refCounter_)) {
    close();
    delete refCounter_;
  }

  --socketCount;
  if (!socketCount) end();
}

Socket::Socket(const Socket& o) {
  refCounter_=o.refCounter_;
  (*refCounter_)++;
  s_         =o.s_;

  socketCount++;
}

Socket& Socket::operator=(Socket& o) {
  (*o.refCounter_)++;

  refCounter_=o.refCounter_;
  s_         =o.s_;

  socketCount++;

  return *this;
}

void Socket::close() {
  closesocket(s_);
}

std::string Socket::receiveBytes() {
  std::string ret;
  char buf[1024];
 
  while (1) {
    u_long arg = 0;
    if (ioctlsocket(s_, FIONREAD, &arg) != 0)
      break;

    if (arg == 0)
      break;

    if (arg > 1024) arg = 1024;

    int rv = recv (s_, buf, arg, 0);
    if (rv <= 0) break;

    std::string t;

    t.assign (buf, rv);
    ret += t;
  }
 
  return ret;
}

std::string Socket::receiveLine() {
  std::string ret;
  while (1) {
    char r;

    switch(recv(s_, &r, 1, 0)) {
      case 0: // not connected anymore;
              // ... but last line sent
              // might not end in \n,
              // so return ret anyway.
        return ret;
      case -1:
        return "";
    }

    ret += r;
    if (r == '\n')  return ret;
  }
}


void Socket::sendLine(std::string s) const {
  s += '\n';
  send(s_,s.c_str(),s.length(),0);
}

void Socket::sendBytes(const std::string& s) {
  send(s_,s.c_str(),s.length(),0);
}


