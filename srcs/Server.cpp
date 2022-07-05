/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 17:34:40 by matthieu          #+#    #+#             */
/*   Updated: 2022/07/05 17:20:32 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netinet/in.h>
#include <unistd.h>
#include "../inc/Server.hpp"

#define MAXUSER 666

Server::Server()
{
}

Server::Server(Server const &src)
{
	this->_server_name = src._server_name;
	this->_port = src._port;
	this->_password = src._password;
}

Server::Server(std::string server_name, int port, char *password): _server_name(server_name), _port(port), _password(password)
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
	this->_password = rhs._password;
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
	if (listen(poll_fd.fd, MAXUSER) < 0)
	{
		std::cout << "Error, failed to grab connection" << std::endl;
		exit(-1);
	}
	_fds.push_front(poll_fd);
}

void	Server::wait_for_event()
{
	int		ret = 0;
	char	buf[4096];

	while (true)
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
			User newUser(newUser_pollfd);
			_users.push_back(newUser);//verifier le constructeur
		}
		std::vector<pollfd>::iterator	it = _fds.begin();
		int index = 1;
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
				msg_parse(buf, index);
				if (read_ret == 0)
				{
					printf("fd = %d et revent = %d\n", it->fd, it->revents);
					close(it->fd);
					it = _fds.erase(it); 
					quit(&_users[index], "");
					continue ;
				}
			}
			index++;
			it++;
		}
		_fds[0].revents = 0;
	}
}

void	Server::msg_parse(char *buf, int index)
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
		if (line.compare(0, 5, "PASS ") == 0)
		{
			pass(&_users[index], string password);

		}
		else
			return ;
		if (line.compare(0, 5, "NICK ") == 0)
			nick(&_users[index], string nickname);
		else if (line.compare(0, 5, "USER ") == 0)
			user(&_users[index], string username, string hostname, string servername, string realname);
		else if (line.compare(0, 5, "QUIT ") == 0)
			quit(&_users[index], string msg);
		else if (line.compare(0, 5, "JOIN ") == 0)
			join(&_users[index], vector<string> & requested_channels);
		else if (line.compare(0, 5, "PART ") == 0)
			part(&_users[index], vector<string> & channels);
		else if (line.compare(0, 5, "MODE ") == 0)
			mode(&_users[index], string requested_channel, vector<string> & operands);
		else if (line.compare(0, 6, "TOPIC ") == 0)
			topic(&_users[index], string channel, string topic);
		else if (line.compare(0, 5, "LIST ") == 0)
			list(&_users[index], vector<string> & channels);
		else if (line.compare(0, 5, "KICK ") == 0)
			kick(&_users[index], string channel, string kickee, string comment);
		else if (line.compare(0, 8, "PRIVMSG ") == 0)
			privmsg(&_users[index], vector<string> & recipients, string msg);
		i++;
		line.erase();
	}
}
