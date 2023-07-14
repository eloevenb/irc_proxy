/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eloevenb <eloevenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 01:32:48 by eloevenb          #+#    #+#             */
/*   Updated: 2023/07/14 03:05:50 by eloevenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "Proxy.class.hpp"

int	main(int argc, char *argv[])
{
	if (argc != 4)
	{
		std::cerr << "Usage : " << argv[0] << " <server ip> <client port> <server port>\n";
		return (1);
	}
	Proxy	ircProxy(atoi(argv[2]), atoi(argv[3]), argv[1]);
	ircProxy.forward();
	return (0);
}