/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 13:16:19 by mservage          #+#    #+#             */
/*   Updated: 2022/07/04 15:39:25 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../inc/Server.hpp"
#include <unistd.h>
#include <stdlib.h>
//#include "User.hpp"

int	main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Wrongs arguments" << std::endl;
		return (1);
	}

	Server		server("ServEau", atoi(av[1]), av[2]);
	server.init_listen();
	server.wait_for_event();
	return (0);
}
