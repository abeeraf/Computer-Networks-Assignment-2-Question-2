#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
using namespace std;
#define PORT 5000

string SetTime()
{
	time_t now = time(0);
	string str;
	tm *ltm = localtime(&now);
	str= to_string(1 + ltm->tm_mon) +"/"+ to_string(ltm->tm_mday) +"/" +to_string((1900 + ltm->tm_year)-2000)+ " ";
	str+= to_string(ltm->tm_hour) +":"+ to_string(ltm->tm_min) +":" +to_string(ltm->tm_sec);
	return str;
}

int main() {

	int listenfd;
	int len; 
	char buffer[1000];
    	struct sockaddr_in cliaddr[3]; 
	struct sockaddr_in servaddr; 
	bzero(&servaddr, sizeof(servaddr)); 

	// Create a UDP Socket 
	listenfd = socket(AF_INET, SOCK_DGRAM, 0);		 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_family = AF_INET; 

	// bind server address to socket descriptor 
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
	
	for(int i=0 ; i<3 ;i++)
	{	
		//Connection with Clients
		len = sizeof(cliaddr[i]);
		int n = recvfrom(listenfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr[i],(socklen_t*)&len); 	//Receive from 1
		buffer[n] = '\0'; 
		puts(buffer);
	}
	
	/////////////////////////////////////////////FUNCTIONALITY/////////////////////////////////////////
	for(int i=0 ; i<3 ;i++)
{	
	char buf[200];
	string str;
	fstream obj;
	
	recvfrom(listenfd, buf, sizeof(buf), 0, (struct sockaddr*)&cliaddr[i],(socklen_t*)&len);
	cout<<buf<<endl;
	
	obj.open("Log.txt", std::ios_base::app);// add to log
	obj<<SetTime()<<endl;
	obj.close();
	
	//str="";//making it Null
	if (buf[0]=='L')// fetch log of server access 
	{	
		obj.open("Log.txt");
		string str2;
		while(getline(obj,str2))
		str+=str2+"\n";
		obj.close();
	}
	else if (buf[0]=='I')// fetch invoice 
	{	
		obj.open("Assignment 1.txt");
		string str2;
		int x=1;
		while(getline(obj,str2))
		{
			if (str2[0]==buf[1] && str2[1]==buf[2] && str2[2]==buf[3] && str2[3]==buf[4] && str2[4]==buf[5] && str2[5]==buf[6])
			str+=to_string(x++)+" "+str2+"\n";
		}
		obj.close();
	}
	else if (buf[0]=='N')// Exit Program
	{	
		str="Program Ended";
	}	
	else if (buf[0]=='D')// Delete Record 
	{	
		obj.open("Assignment 1.txt");
		string str2;
		int x=0;
		while(getline(obj,str2))
		{	
			if ("D"+str2==buf)
			getline(obj,str2);//skip that line
	
			str+=str2+"\n";
		}
		obj.close();

		std::ofstream ofs;
		ofs.open("Assignment 1.txt", std::ofstream::out | std::ofstream::trunc);
		ofs<<str;
		ofs.close();

		str="Record deleted";
	}		
	else // add record to invoice
	{
		obj.open("Assignment 1.txt");
		string str2;
		str="";
		int x=0;
		while(getline(obj,str2))
		{	
			
			if (!x && (str2[0]==buf[0] && str2[1]==buf[1] && str2[2]==buf[2] && str2[3]==buf[3] && str2[4]==buf[4] && str2[5]==buf[5])){
			str=str+buf+"\n";//add the line above 
			x=1;}
	
			str+=str2+"\n";
		}
		obj.close();

		obj.open("Assignment 1.txt");//rewrite file with it
		obj<<str;
		obj.close();
		str="Record Added";
		for(int j=0 ; j<3 ;j++)
		sendto(listenfd, "Record Added", sizeof("Record Added"), 0, (struct sockaddr*)&cliaddr[j], sizeof(cliaddr[j]));
		
	}
	//cout<<str<<endl;
	char server_message2[str.length()+1];
	strcpy(server_message2, str.c_str());
	sendto(listenfd, server_message2, sizeof(server_message2), 0, (struct sockaddr*)&cliaddr[i], sizeof(cliaddr[i]));//sending Log or Innvoice
	// close the socket
}
	close(listenfd);
	
	return 0;
}

