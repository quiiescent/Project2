#include <iostream>
// Stuff for AJAX
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

//Stuff for pipes
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
using namespace cgicc; // Needed for AJAX functions.

// fifo for communication
string receive_fifo = "Namereply";
string send_fifo = "Namerequest";

int main() {
  Cgicc cgi;    // Ajax object
  char *cstr;
  // Create AJAX objects to recieve information from web page.
  form_iterator name = cgi.getElement("name");

  // create the FIFOs for communication
  Fifo recfifo(receive_fifo);
  Fifo sendfifo(send_fifo);

  // Call server to get results
  string stname = **name;
  string message = stname;
  transform(message.begin(), message.end(), message.begin(), ::tolower);
  sendfifo.openwrite();
  sendfifo.send(message);
  
  /* Get a message from a server */
  recfifo.openread();
  cout << "Content-Type: text/plain\n\n";
  string results = "";
  do {
  	results = recfifo.recv();
  	if (results.find("$END") != -1) {
  	break;
  	}
  	cout << results;
  } while (1);
  recfifo.fifoclose();
  sendfifo.fifoclose();
  
return 0;
}
