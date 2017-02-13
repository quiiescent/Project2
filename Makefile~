#MakeFile to build and deploy the Sample US CENSUS Name Data using ajax
# For CSC3004 Software Development

# Put your user name below:
USER= skon

CC= g++

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -g

RM= /bin/rm -f

all: nameserver testclient namefetchajax PutCGI PutHTML
#all: nameserver testclient 

testclient.o: testclient.cpp fifo.h
	$(CC) -c $(CFLAGS) testclient.cpp

nameserver.o: nameserver.cpp fifo.h
	$(CC) -c $(CFLAGS) nameserver.cpp

namefetchajax.o: namefetchajax.cpp fifo.h
	$(CC) -c $(CFLAGS) namefetchajax.cpp

testclient: testclient.o fifo.o
	$(CC) testclient.o fifo.o -o testclient

nameserver: nameserver.o fifo.o
	$(CC) nameserver.o  fifo.o -o nameserver

fifo.o:		fifo.cpp fifo.h
		g++ -c fifo.cpp
namefetchajax: namefetchajax.o  fifo.h
	$(CC) namefetchajax.o  fifo.o -o namefetchajax -L/usr/local/lib -lcgicc

PutCGI: namefetchajax
	chmod 757 namefetchajax
	cp namefetchajax /usr/lib/cgi-bin/$(USER)_namefetchajax.cgi 

	echo "Current contents of your cgi-bin directory: "
	ls -l /usr/lib/cgi-bin/

PutHTML:
	cp name_stats_ajax.html /var/www/html/class/softdev/$(USER)
	cp name_stats_ajax.css /var/www/html/class/softdev/$(USER)

	echo "Current contents of your HTML directory: "
	ls -l /var/www/html/class/softdev/$(USER)

clean:
	rm -f *.o name_stats_ajax nameserver testclient
