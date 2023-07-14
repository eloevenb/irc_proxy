/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Proxy.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eloevenb <eloevenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 01:53:08 by eloevenb          #+#    #+#             */
/*   Updated: 2023/07/14 02:40:13 by eloevenb         ###   ########.fr       */
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

Proxy::Proxy(int clientPort, int serverPort, std::string & destinationIp) throw(SocketCreationFailureException, SocketBindFailureException)
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
	_serverAddress.sin_addr.s_addr = inet_addr(_destinationIp);
	
	_clientAddress.sin_family = AF_INET;
	_clientAddress.sin_port = htons(_clientPort);
	_clientAddress.sin_addr.s_addr = INADDR_ANY;

	if (bind(_clientSocketFd, (struct sockaddr *)&_clientAddress, sizeof(_clientAddress)) == -1)
		throw SocketBindFailureException();
	if (bind(_serverSocketFd, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress)) == -1)
		throw SocketBindFailureException();

	std::cout << "Successfuly created and bound sockets!\n";
}

Proxy::Proxy(const Proxy & rhs) throw(SocketCreationFailureException, SocketBinFailureException) : Proxy(rhs._clientPort, rhs._serverPort, rhs._destinationIp) {}

Proxy & Proxy::operator=(const Proxy & rhs) throw(SocketCreationFailureException, SocketBinFailureException)
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
	this->_serverAddress.sin_addr.s_addr = inet_addr(this->_destinationIp);
	
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

int	Proxy::getClientSocketFd() const
{
	return (_clientSocketFd);
}

int	Proxy::getServerSocketFd() const
{
	return (_serverSocketFd);
}

void	Proxy::listen(int backlog=1) throw(SocketListenFailureException)
{
	if (listen(_clientSocketFd, backlog) == -1)
		throw SocketListenFailureException();
	if (listen(_serverSocketFd, 1) == -1)
		throw SocketListenFailureException();
}

