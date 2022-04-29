/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 11:24:21 by matthieu          #+#    #+#             */
/*   Updated: 2022/04/29 15:02:39 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netinet/in.h>
#include "Server.hpp"

Server::Server()
{
}

Server::Server(Server const &src)
{
	this->_server_name = src._server_name;
	this->_port = src._port;
	for (int i = 0; i < src._users.size(); i++)
		this->_users.push_back(src._users[i]);
	for (int i = 0; i < src._channels.size(); i++)
		this->_channels.push_back(src._channels[i]);
}

Server::Server(std::string server_name, int port): _server_name(server_name), _port(port)
{
}

Server::~Server()
{
}

Server	&Server::operator=(Server const &rhs)
{
	this->_server_name = rhs._server_name;
	this->_port = rhs._port;
	for (int i = 0; i < rhs._users.size(); i++)
		this->_users.push_back(rhs._users[i]);
	for (int i = 0; i < rhs._channels.size(); i++)
		this->_channels.push_back(rhs._channels[i]);
	return (*this);
}

int	Server::getPort(void) const
{
	return (this->_port);
}

std::string	Server::getServerName(void) const
{
	return (this->_server_name);
}

void	Server::addNewUser(std::string name, std::string password)
{
	sockaddr_in	address;
	
	_users.push_back(User(name, password));
	_fds.push_back(_users[_users.size() - 1].getPollFd());
	address.sin_family = AF_INET;
	address.sin_port = htons(6666); //port de notre serveur
	address.sin_addr.s_addr = INADDR_ANY;
	bind(_fds[_fds.size() - 1].fd, (struct sockaddr *)&address, sizeof(address));
}
