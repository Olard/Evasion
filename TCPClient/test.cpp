#include "../Evasion-Server/src/network/ClientSocket.h"

#include <process.h>
#include <iostream>

using namespace std;

unsigned __stdcall receiver(void* parameter) {
	Socket* socket = (Socket*) parameter;
	
	while (1) {
		std::string message = socket->receiveLine();
		if (message != "\n") {
			if (message == "exit") break;
			cout << "Received line: " << message;
			cout.flush();
		}
	}
	return 0;
}

int main() {

  try {
    ClientSocket s("127.0.0.1", 27015);
	
	unsigned ret;
    _beginthreadex(0,0,receiver,(void*) &s,0,&ret);
	
    while (1) {
	
      string l;
	  cin >> l;
      if (l == "exit") break;
      s.sendLine(l);
	  cout << "Sent the line: " << l << endl;
    }

  }
  catch (NetworkException &ex) {
    cerr << ex.what() << endl;;
  }

  return 0;
}