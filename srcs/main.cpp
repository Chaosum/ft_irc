/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 13:16:19 by mservage          #+#    #+#             */
/*   Updated: 2022/05/16 13:57:58 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Server.hpp"
#include <unistd.h>
//#include "User.hpp"

int	main(int ac, char **av, char **env)
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
