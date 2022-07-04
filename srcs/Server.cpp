/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD
/*   Created: 2022/06/03 13:28:47 by matthieu          #+#    #+#             */
/*   Updated: 2022/06/12 17:32:00 by matthieu         ###   ########.fr       */
=======
/*   Created: 2022/04/28 11:24:21 by matthieu          #+#    #+#             */
/*   Updated: 2022/06/07 22:02:32 by lgaudet-         ###   ########.fr       */
>>>>>>> origin/leto
/*                                                                            */
/* ************************************************************************** */

#include <netinet/in.h>
<<<<<<< HEAD
#include <unistd.h>
#include "Server.hpp"
=======
#include "../inc/Server.hpp"
>>>>>>> origin/leto

Server::Server()
{
}

Server::Server(Server const &src)
{
	this->_server_name = src._server_name;
	this->_port = src._port;
	this->_serv_password = src._serv_password;
}

Server::Server(std::string server_name, int port, char *password): _server_name(server_name), _port(port), _serv_password(password)
{
	this->_addr_size = sizeof(_address);
}

Server::~Server()
{
}

Server	&Server::operator=(Server const &rhs)
{
	this->_server_name = rhs._server_name;
	this->_port = rhs._port;
	this->_serv_password = rhs._serv_password;
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

std::string	Server::getServerPassword(void) const
{
	return (this->_serv_password);
}


std::vector<pollfd> Server::getFds()
{
	return (this->_fds);
}

void	Server::init_listen()
{
	pollfd		poll_fd;

	poll_fd.fd = socket(AF_INET, SOCK_STREAM, 0);
	this->_address.sin_family = AF_INET;
	this->_address.sin_port = htons(this->_port); //port de notre serveur
	this->_address.sin_addr.s_addr = INADDR_ANY;
	poll_fd.events = POLLIN|POLLOUT;
	poll_fd.revents = 0;
	if (bind(poll_fd.fd, (struct sockaddr *)&_address, _addr_size) == -1)
	{
		std::cout << "Error, failed to bind" << std::endl;
		exit(-1);
	}
	if (listen(poll_fd.fd, 666) < 0) // 666?
	{
		std::cout << "Error, failed to grab connection" << std::endl;
		exit(-1);
	}
	_fds.push_back(poll_fd);
}

void	Server::wait_for_event()
{
	int		ret = 0;
	char	buf[4096];

	while (1)
	{
		ret = poll(&(_fds[0]), _fds.size(), 0);
		if (ret < 0)
		{
			std::cout << "Poll error" << std::endl;
			break ;
		}
		else if (ret == 0)
			continue ;
		else if (_fds[0].revents & POLLIN)
		{
			std::cout << "New User accepted" << std::endl;
			int new_user = accept(_fds[0].fd, (struct sockaddr *)&_address, (socklen_t *)&_addr_size);
			pollfd	newUser_pollfd;
			newUser_pollfd.fd = new_user;
			newUser_pollfd.events = POLLIN|POLLOUT;
			newUser_pollfd.revents = 0;
			_fds.push_back(newUser_pollfd);
		}
		std::vector<pollfd>::iterator	it = _fds.begin();
		it++;
		int read_ret = 1;
		while (it != _fds.end())
		{
			if (it->revents & POLLIN)
			{
				read_ret = 1;
				for (int i = 0; i < 4096; i++)
					buf[i] = 0;
				read_ret = read(it->fd, buf, 4096); //read the message
				printf("%s", buf); 
				msg_parse(buf);
				if (read_ret == 0)
				{
					printf("fd = %d et revent = %d\n", it->fd, it->revents);
					close(it->fd);
					it = _fds.erase(it);
					continue ;
				}
			}
			it++;
		}
		_fds[0].revents = 0;
	}
}

void	Server::msg_parse(char *buf)
{
	int i = 0;
	std::string line;

	while (buf[i])
	{
		while (buf[i] != '\n' && buf[i] != 0 & buf[i] != '\r')
		{
			line = line + buf[i];
			i++;
		}
		printf("line = %s\n", line.c_str());
		if (line == "PASS")
			;//fonction PASS
		if (line == "NICK")
			;//fonction NICK
		if (line == "USER")
			;//fonction USER
		line.clear();
		i++;
	}
}

