/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Proxy.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eloevenb <eloevenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 01:53:08 by eloevenb          #+#    #+#             */
/*   Updated: 2023/07/14 03:24:45 by eloevenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Proxy.class.hpp"

const char *Proxy::SocketCreationFailureException::what() const throw()
{
	return ("Failed to initialize socket");
}

const char	*Proxy::SocketBindFailureException::what() const throw()
{
	return ("Failed to bind socket");
}

const char	*Proxy::SocketListenFailureException::what() const throw()
{
	return ("Failed to listen");
}

Proxy::Proxy() {}

Proxy::Proxy(int clientPort, int serverPort, const char *destinationIp)
{
	_clientSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_clientSocketFd < 0)
		throw SocketCreationFailureException();
	_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocketFd < 0)
		throw SocketCreationFailureException();

	_destinationIp = destinationIp;

	_clientPort = clientPort;
	_serverPort = serverPort;
	
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_port = htons(_serverPort);
	_serverAddress.sin_addr.s_addr = inet_addr(destinationIp);
	
	_clientAddress.sin_family = AF_INET;
	_clientAddress.sin_port = htons(_clientPort);
	_clientAddress.sin_addr.s_addr = INADDR_ANY;

	if (bind(_clientSocketFd, (struct sockaddr *)&_clientAddress, sizeof(_clientAddress)) == -1)
		throw SocketBindFailureException();
	if (bind(_serverSocketFd, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress)) == -1)
		throw SocketBindFailureException();

	std::cout << "Successfuly created and bound sockets!\n";
}

Proxy::Proxy(const Proxy & rhs) : Proxy(rhs._clientPort, rhs._serverPort, rhs._destinationIp.c_str()) {}

Proxy & Proxy::operator=(const Proxy & rhs)
{
	this->_clientSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_clientSocketFd < 0)
		throw SocketCreationFailureException();
	this->_serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_serverSocketFd < 0)
		throw SocketCreationFailureException();

	this->_destinationIp = rhs._destinationIp;

	this->_clientPort = rhs._clientPort;
	this->_serverPort = rhs._serverPort;
	
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_port = htons(this->_serverPort);
	this->_serverAddress.sin_addr.s_addr = inet_addr(this->_destinationIp.c_str());
	
	this->_clientAddress.sin_family = AF_INET;
	this->_clientAddress.sin_port = htons(this->_clientPort);
	this->_clientAddress.sin_addr.s_addr = INADDR_ANY;

	if (bind(this->_clientSocketFd, (struct sockaddr *)&this->_clientAddress, sizeof(this->_clientAddress)) == -1)
		throw SocketBindFailureException();
	if (bind(this->_serverSocketFd, (struct sockaddr *)&this->_serverAddress, sizeof(this->_serverAddress)) == -1)
		throw SocketBindFailureException();

	std::cout << "Successfuly created and bound sockets!\n";
	return (*this);
}

Proxy::~Proxy()
{
	close(_clientSocketFd);
	close(_serverSocketFd);	
}

int	Proxy::getClientSocketFd() const
{
	return (_clientSocketFd);
}

int	Proxy::getServerSocketFd() const
{
	return (_serverSocketFd);
}

void	Proxy::forward()
{
	if (listen(_clientSocketFd, 10) == -1)
		throw SocketListenFailureException();
	if (listen(_serverSocketFd, 1) == -1)
		throw SocketListenFailureException();

	struct sockaddr_in clientAddress;
	socklen_t clientAddressLength = sizeof(clientAddress);
	int clientConnection = accept(_clientSocketFd, (struct sockaddr *)&clientAddress, &clientAddressLength);
	if (clientConnection == -1) 
		throw SocketListenFailureException();

	int serverConnection = connect(_serverSocketFd, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress));
	if (serverConnection == -1) 
		throw SocketListenFailureException();
	
	fd_set readFds;
	int maxFd = std::max(clientConnection, serverConnection) + 1;
	char buffer[1024];

	while (true) {
		FD_ZERO(&readFds);
		FD_SET(clientConnection, &readFds);
		FD_SET(serverConnection, &readFds);

		int selectResult = select(maxFd, &readFds, nullptr, nullptr, nullptr);
		if (selectResult == -1)
		{
			std::cerr << "Select failed\n";
			break ;
		}

		if (FD_ISSET(clientConnection, &readFds)) {
			// Data available from client, forward to server
			int bytesRead = recv(clientConnection, buffer, sizeof(buffer), 0);
			if (bytesRead <= 0)
			{
				std::cerr << "received empty buffer from client\n";
				break ;
			}
			send(serverConnection, buffer, bytesRead, 0);
			buffer[bytesRead] = '\0';
			std::cout << "[INCOMING PACKET FROM CLIENT]\n" << buffer << std::endl;
		}

		if (FD_ISSET(serverConnection, &readFds)) {
			// Data available from server, forward to client
			int bytesRead = recv(serverConnection, buffer, sizeof(buffer), 0);
			if (bytesRead <= 0)
			{
				std::cerr << "received empty buffer from server\n";
			}
			send(clientConnection, buffer, bytesRead, 0);
			buffer[bytesRead] = '\0';
			std::cout << "[INCOMING PACKET FROM SERVER]\n" << buffer << std::endl;
		}
	}
	close(clientConnection);
	close(serverConnection);
}

