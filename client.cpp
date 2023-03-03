#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include<stdlib.h>
#include <arpa/inet.h> 
using namespace std;
#define PORT 5000


int main() {
	
	
	char buffer[1000]; 
	char message[50] = "Client connecting to server."; 
	int sockfd;
	int n; 
	struct sockaddr_in servaddr; 
	string invoice;
	
	// clear servaddr 
	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_family = AF_INET; 
	
	// create datagram socket 
	sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
	
	// connect to server 
	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
	{ 
		printf("\n Error : Connect Failed \n"); 
		exit(0); 
	}
	sendto(sockfd, "Connecting", sizeof("Connecting"), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
	
	int select;
	char buf[1000];
	cout<<endl<<"Select Number"<<endl;
	cout<<"(1)Add Record"<<endl;
	cout<<"(2)Delete Record"<<endl;
	cout<<"(3)Read Invoice"<<endl;
	cout<<"(4)Ask For Access Log"<<endl;
	cout<<"(5)End Program"<<endl;
	cin>>select;

	char record[256];
	
	if (select == 1)
	{
		cout<<"Enter Record to Add in format -->(Invoice no)(Description)(Quantity)(Price in $)(Customer id)(Country)"<<endl;
		cin.ignore();
		cin.getline(record,256);
		sendto(sockfd, record, sizeof(record), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
	}
	else if (select == 2)
	{ 
		cout<<"Enter Record to Delete in format -->D(Invoice no)(Description)(Quantity)(Price in $)(Customer id)(Country)"<<endl;
		cin.ignore();
		cin.getline(record,256);
		sendto(sockfd, record, sizeof(record), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
	}
	else if (select == 3)
	{
		cout<<"Enter Invoice Number for Server to fetch in format--> I(Invoice Number): ";
		for(int x = 0 ; x < 7 ; x++)
		cin>>record[x];
		sendto(sockfd, record, sizeof(record), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
	}
	else if (select == 4)
	{
		sendto(sockfd, "Log", sizeof("Log"), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
	}
	else if (select == 5)
	{
		record[0]='N'; //terminat program
		sendto(sockfd, record, sizeof(record), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
	}	
	recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&servaddr,(socklen_t*)&n); 
	cout<<buf<<endl; 

	close(sockfd);

	return 0;
}
