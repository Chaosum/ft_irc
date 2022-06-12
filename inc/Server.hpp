/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 18:07:17 by mservage          #+#    #+#             */
/*   Updated: 2022/06/10 15:59:24 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
//#include "User.hpp"
//#include "Channel.hpp"

/* server contient:
	la liste des channel
	la liste des users
	la liste des fds des users (a mettre en struct pollfd pour poll)
	*/

class Server
{
private:
	std::string		_server_name;
	std::string		_serv_password;
	int				_port;
	sockaddr_in		_address;
	int				_addr_size;


	//std::vector<Channel>	_channels;
	//std::vector<User>		_users;
	std::vector<pollfd>		_fds;

public:
	Server();
	Server(Server const &src);
	Server(std::string server_name, int port, char *password);
	~Server();

	Server	&operator=(Server const &rhs);
	int	getPort(void) const;
	std::string	getServerName(void) const;
	std::string	getServerPassword(void) const;
	std::vector<pollfd> getFds();
	void	init_listen();
	void	wait_for_event();
	void	msg_parse(char *buf);
};
