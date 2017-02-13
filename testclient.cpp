#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "fifo.h"

using namespace std;
string receive_fifo = "Namereply";
string send_fifo = "Namerequest";

int main() {
  string message, aWord;
  string reply;

  while (1) {
	cout << "Enter a word to search for:";
	cin >>  aWord;
	transform(aWord.begin(), aWord.end(), aWord.begin(), ::tolower);

	
	// create the FIFOs for communication
	Fifo recfifo(receive_fifo);
	Fifo sendfifo(send_fifo);

	message = aWord;
	sendfifo.openwrite();
	sendfifo.send(message);
	cout << "Sent:" << message << endl;

	recfifo.openread();
	do {
		reply = recfifo.recv();
		if (reply.find("$END") != -1) {
		break;
		}
		cout << "Server sent: " << reply << endl;
	} while (1);
	recfifo.fifoclose();
	sendfifo.fifoclose();
	cout << "Loop closed, fifos closed. All is well." << endl;
  }
}
