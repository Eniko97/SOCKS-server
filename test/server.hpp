#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>
#include <iostream>

const int MAX_CONNECTION = 5;

class Server
{
private:
	int sockfd;
	int portno;
	struct sockaddr_in server_addr, client_addr;
	socklen_t client_len;
	char buffer[1024];
	
public:
	Server(int port = 5487)
		: portno(port)
	{
		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0)
		{
			std::cerr << "Server socket open failed.\n";
			exit(-1);
		}

		memset(&server_addr, '0', sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = INADDR_ANY;
		server_addr.sin_port = htons(portno);

		if (bind(sockfd, (sockaddr *) &server_addr, sizeof(server_addr)) < 0)
		{
			std::cerr << "Server bind failed.\n";
			exit(-1);
		}

		listen(sockfd, MAX_CONNECTION);

		client_len = sizeof(client_addr);

	}

	int accept_one()
	{
		int clientfd;

		if ((clientfd = accept(sockfd, (sockaddr *) &client_addr, &client_len)) < 0)
		{
			std::cerr << "Server accept failed.\n";
			exit(-1);
		}

		return clientfd;
	}

	void run()
	{
		while (true)
		{
			int clientfd = accept_one();
			
			int pid = fork();
			if (pid < 0)
			{
				std::cerr << "Server fork failed.\n";
				exit(-1);
			}

			if (pid == 0)
			{
				close(sockfd);

				//Service::service(clientfd, buffer, 1023);
			}
			else
			{
				close(clientfd);
			}
		}
	}

};

#endif
