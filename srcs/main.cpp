/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 13:16:19 by mservage          #+#    #+#             */
/*   Updated: 2022/05/04 11:47:28 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Server.hpp"
#include "User.hpp"

int	main(int ac, char **av, char **env)
{
	if (ac != 2)
	{
		std::cout << "Wrongs arguments" << std::endl;
		return (1);
	}
	sockaddr_in	address;
	Server		server("ServO", 6666);
	pollfd		poll_fd;
	int			listen_fd;

	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_port = htons(6666); //port de notre serveur
	address.sin_addr.s_addr = INADDR_ANY;
	poll_fd.fd = listen_fd;
	poll_fd.events = POLLIN|POLLOUT;
	poll_fd.revents = 0;
	if (bind(poll_fd.fd, (struct sockaddr *)&address, sizeof(address)) == -1)
	{
		std::cout << "Error, failed to bind" << std::endl;
		exit(-1);
	}
	if (listen(poll_fd.fd, 666) < 0)
	{
		std::cout << "Error, failed to grab connection" << std::endl;
		exit(-1);
	}
	server.getFds().push_back(poll_fd);
	while (42)
	{
		
	}
}
