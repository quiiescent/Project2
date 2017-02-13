/***************************************************************************
* nameservercpp  -  Program to serve of last name statistics
 *
* copyright : (C) 2009 by Jim Skon
*
* This program runs as a background server to a CGI program, providinging US Census
* Data on the frequency of names in response to requestes.  The rational for this 
* is to demonstrait how a background process can serve up information to a web program,
* saving the web program from the work of intiallizing large data structures for every
* call.
* 
* 
*
***************************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>
#include "fifo.h"

using namespace std;

/* Fifo names */
string receive_fifo = "Namerequest";
string send_fifo = "Namereply";

int makeTheMap(map<string, vector<int> >& myMap, ifstream& is, string filename, int& pos) {

    is.open(filename.c_str()); //open the file    
    int loc = 0;
    string line;
    bool theCodeRan = false;
    while (getline(is, line)) {
        theCodeRan = true;

        //set the line to all lowercase
        transform(line.begin(), line.end(), line.begin(), ::tolower);

        pos = 0; //track the position in the line
        string newWord;
        int length = line.length();

        //run while the position in the line isn't greater than the length of the line
        while (pos <= line.length()) {
            newWord = ""; //clear newWord

            //scan for an alpha character or apostrophe while pos <= length
            while (!isalpha(line[pos]) && (line[pos] != '\'' && pos <= length)) {
                pos++;
            }

            //once we find an alpha character/apostrophe, keep concatenating them until we find a non
            //alpha character/apostrophe while pos <= length
            while (isalpha(line[pos]) || (line[pos] == '\'' && pos <= length)) {
                newWord = newWord + line[pos];
                pos++;
            }

            myMap[newWord].push_back(loc); //map newWord to loc, or add loc to the existing vector
        }
        loc = is.tellg(); //set loc to the next line
    }
    is.close(); //close the file
    if (theCodeRan = true) {
        return 1;
    } else {
        return 0;
    }
}

vector <int> indexSearch(map<string, vector<int> >& myMap, string word) {
    map<string, vector<int> >::iterator it; // iterator for find                                                                
    vector<int> blank; // return for no matches                                                                                  
    /* find the word and get the vector of references */
    /* First use find, so as to NOT create a new entry */
    it = myMap.find(word);
    if (it == myMap.end()) {
        return (blank);
    } else {
        return (myMap[word]);
    }
}

/* Server main line,create name MAP, wait for and serve requests */
int main() {
  
  map <string, vector<int> > myMap;
  string filename = "/home/class/SoftDev/Shakespeare/Shakespeare.txt";
  string inMessage, outMessage, aWord, result;
  int pos = 0;
  ifstream ssfile;

  // Create the map of Shakespeare words
    if (makeTheMap(myMap, ssfile, filename, pos) != 1) {
        cout << "Error Loading Database" << endl;
        exit(0);
    }
  
  cout << "Name data loaded!" << endl;

  // create the FIFOs for communication
  Fifo recfifo(receive_fifo);
  Fifo sendfifo(send_fifo);
  
  ssfile.open(filename.c_str()); //open the file

    while (1) {

	cout << "Ready to receive a message!" << endl;
    recfifo.openread();
    inMessage = recfifo.recv();

	cout << "Received this message from server: " << inMessage << endl;
	
	aWord = inMessage;
    vector<int> wordIndices = indexSearch(myMap, aWord); //get the vector mapped to aWord
    vector<int>::iterator myIntVectorIterator;
    
    sendfifo.openwrite();

    for (myIntVectorIterator = wordIndices.begin();
                myIntVectorIterator < wordIndices.end();
                myIntVectorIterator++) {
        	
        	//cout << "3" << endl;

            int i = *myIntVectorIterator;
            ssfile.seekg(i, ssfile.beg); //seekg to the line

	        //cout << "4" << endl;

            string line2 = "";
            getline(ssfile, line2); //get the line
        
            //cout << "5" << endl;

            outMessage = "<p>" + line2 + "</p>";
            //cout << "6" << endl;
        	sendfifo.send(outMessage); //send the message
        	cout << "Sent this message to server: " << outMessage << endl;
        }
        string endstring = "$END";
        sendfifo.send(endstring); //let it know that we're done
        cout << "Sent this message to server: " << endstring << endl;       	
	sendfifo.fifoclose();
	recfifo.fifoclose();
  }
}
