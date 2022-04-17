//============================================================================
// Name        : SerwiceSerwer.cpp
// Author      : Kacper Krzekotowski
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <vector>
#include "fcntl.h"
#include <unistd.h>

struct Session{

	Session(int socket):socket{socket}{}


	template<class T>
	void operator<<(T && data){
		int size = sizeof data;
		int trans =0;
		if(trans = send(socket,(void *)data,size,0) == -1)std::cout<<"Connection was Closed \n";
	}

int socket;
};



class Serwer {
public:
	Serwer(){
		memset(&hints,0,sizeof hints);
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE;
		getaddrinfo(NULL,"2409",&hints,&res);
		mainSocket =0;
	}

	void create_socket();
	void bind();
	void listen();
	void accept();
	void * get_in_addr(struct sockaddr *sa);
private:
struct addrinfo hints ,*res;
struct sockaddr_storage client;
char s[INET6_ADDRSTRLEN];
int mainSocket;
public: std::vector<Session> sesionVec;
};


void Serwer::create_socket(){
	mainSocket = socket(this->res->ai_family,res->ai_socktype,res->ai_protocol);
	if(mainSocket ==-1) std::cout<<"Can't Create main socket \n";
	fcntl(mainSocket, F_SETFL, O_NONBLOCK);
	 freeaddrinfo(res);
}


void Serwer::bind(){
	if(::bind(mainSocket,this->res->ai_addr,res->ai_addrlen)==-1) std::cout<<"Can not bind \n";
}

void Serwer::listen(){
	if(::listen(mainSocket,5)==-1)std::cout<<"Error Listening \n ";
}

void * Serwer::get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


void Serwer::accept(){
	socklen_t clientSize = sizeof client;
	int newFd = ::accept(mainSocket,(struct sockaddr *)&client,&clientSize);
	if(newFd == -1);//std::cerr<<"Invalid client fd\n";
	else {
        inet_ntop(client.ss_family,
            get_in_addr((struct sockaddr *)&client),
            s, sizeof s);
		printf("Connection from %s \n",s);
		sesionVec.push_back(Session(newFd));
	}
}


int main() {

	Serwer srv{};
	srv.create_socket();
	srv.bind();
	srv.listen();
	while(1){
	srv.accept();

	for(auto &ses:srv.sesionVec){

		ses<<"Some Data \n";
	}

	}

}
