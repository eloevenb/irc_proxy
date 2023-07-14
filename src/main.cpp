/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eloevenb <eloevenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 01:32:48 by eloevenb          #+#    #+#             */
/*   Updated: 2023/07/14 01:44:33 by eloevenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int	main(int argc, char *argv[])
{
	int socketFd;

	if (argc != 3)
	{
		std::cerr << "Usage : " << argv[0] << " <server ip> <port>\n";
		return (1);
	}
	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd < 0)
	{
		std::cerr << "Socket creation failed\n";
		return (1);
	}
	return (0);
}