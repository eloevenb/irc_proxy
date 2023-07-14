/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Proxy.class.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eloevenb <eloevenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 01:45:11 by eloevenb          #+#    #+#             */
/*   Updated: 2023/07/14 03:19:20 by eloevenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class Proxy
{
	public:
		class SocketCreationFailureException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class SocketBindFailureException : public std::exception
		{
			public:
				virtual const char	*what() const throw();
		};

		class SocketListenFailureException : public std::exception
		{
			public:
				virtual const char	*what() const throw();
		};

		Proxy(int clientPort, int serverPort, const char *destinationIp);
		Proxy(const Proxy & rhs);
		Proxy & operator=(const Proxy & rhs);
		~Proxy();

		int	getClientSocketFd() const;
		int	getServerSocketFd() const;

		void	forward();
		
	private:
		Proxy();

		std::string			_destinationIp;

		int					_clientSocketFd;
		int					_serverSocketFd;

		int					_clientPort;
		int					_serverPort;
		
		struct sockaddr_in	_serverAddress;
		struct sockaddr_in	_clientAddress;
};