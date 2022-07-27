/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 17:34:40 by matthieu          #+#    #+#             */
/*   Updated: 2022/07/27 15:16:29 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netinet/in.h>
#include <unistd.h>
#include "../inc/Server.hpp"

#define MAXUSER 666

Server::Server(): _password("blabla")
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
	_fds.push_back(poll_fd);
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
		int index = 0;
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
					_users.erase( _users.begin() + index);
					continue ;
				}
			}
			index++;
			it++;
		}
		_fds[0].revents = 0;
	}
}

std::string	Server::getNextWord(std::string line, int *i, std::string &tmp) const
{
	tmp.clear();
	while (line[*i] == ' ')
	{
		*i = *i + 1;
	}
	if (line[*i] == ':')
	{
		while (line[*i] != '\r' && line[*i] != '\n' && line[*i] != 0)
		{
			tmp = tmp + line[*i];
			*i = *i + 1;
		}
	}
	else
	{
		while (line[*i] != ' ' && line[*i] != '\r' && line[*i] != 0)
		{
			tmp = tmp + line[*i];
			*i = *i + 1;
		}
	}
	return (tmp);
}

std::vector<std::string>	Server::getNextVector(std::string line, int *i, int lastword)
{
	std::vector<std::string> dest;
	std::string tmp;
	int i_save = 0;
	while (line[*i] != '\r' && line[*i] != 0)
	{
		tmp.clear();
		i_save = *i;
		while (line[*i] == ' ')
			*i = *i + 1;
		if (line[*i] == ':')
			return (dest);
		while (line[*i] != ' ' && line[*i] != 0 && line[*i] != '\r')
		{
			tmp = tmp + line[*i];
			*i = *i + 1;
		}
		dest.push_back(tmp);
	}
	if (lastword)
	{
		*i = i_save;
		dest.erase(dest.end() - 1);
	}
	return (dest);
}

void	Server::msg_parse(char *buf, int index)
{
	int i = 0;
	std::string	line;
	std::string	tmp;
	std::vector<string> temp_vector;
	while (buf[i] != 0) //dernier argument a refaire en verifiant les ':'
	{
		int tmp_i = 0;
		while (buf[i] != '\n' && buf[i] != 0 && buf[i] != '\r')
		{
			line = line + buf[i];
			i++;
		}
		if (line[0] == ':')
			getNextWord(line, &(tmp_i = 1), tmp);
		if (line.compare(0, 5, "PASS ") == 0)
		{
			std::cout << "coucou" << std::endl;
			pass(&_users[index], getNextWord(line, &(tmp_i = 5), tmp));
		}
		else if (line.compare(0, 5, "NICK ") == 0)
		{
			nick(&_users[index], getNextWord(line, &(tmp_i = 5), tmp));
		}
		else if (line.compare(0, 5, "USER ") == 0) // :
		{
			user(&_users[index], getNextWord(line, &(tmp_i = 5), tmp),getNextWord(line, &tmp_i, tmp), getNextWord(line, &tmp_i, tmp), getNextWord(line, &tmp_i, tmp));
		}
		else if (!_command_exists(line))
			printf("connard = %s\n", line.c_str());
		else if (!_users[index].isAuth())
			printf("sakafoutr\n");
		else if (line.compare(0, 5, "QUIT ") == 0) // :
		{
			quit(&_users[index], getNextWord(line, &tmp_i, tmp));
		}
		else if (line.compare(0, 5, "JOIN ") == 0)
		{
			join(&_users[index], temp_vector = getNextVector(line, &(tmp_i = 5), 1));
		}
		else if (line.compare(0, 5, "PART ") == 0) // :
		{
			part(&_users[index], temp_vector = getNextVector(line, &(tmp_i = 5), 1), getNextWord(line, &tmp_i, tmp));
		}
		else if (line.compare(0, 5, "MODE ") == 0)
		{
			mode(&_users[index], getNextWord(line, &(tmp_i = 5), tmp), temp_vector = getNextVector(line, &tmp_i, 0));
		}
		else if (line.compare(0, 6, "TOPIC ") == 0) // :
		{
			topic(&_users[index], getNextWord(line, &(tmp_i = 6), tmp), getNextWord(line, &tmp_i, tmp));
		}
		else if (line.compare(0, 5, "LIST ") == 0)
		{
			list(&_users[index], temp_vector = getNextVector(line, &(tmp_i = 5), 1));
		}
		else if (line.compare(0, 5, "KICK ") == 0) // :
		{
			kick(&_users[index], getNextWord(line, &(tmp_i = 5), tmp), getNextWord(line, &tmp_i, tmp), getNextWord(line, &tmp_i, tmp));
		}
		else if (line.compare(0, 8, "PRIVMSG ") == 0) // :
		{
			privmsg(&_users[index],temp_vector = getNextVector(line, &(tmp_i = 8), 1), getNextWord(line, &tmp_i, tmp));
		}
		else if (line.compare(0, 7, "NOTICE ") == 0)
		{
			notice(&_users[index], getNextWord(line, &(tmp_i = 7), tmp), getNextWord(line, &tmp_i, tmp));
		}
		i = i + 2;
		line.erase();
	}
}

bool	Server::_command_exists(std::string line)
{
		if (line.compare(0, 5, "QUIT ") == 0)
		{
			return true;
		}
		else if (line.compare(0, 5, "JOIN ") == 0)
		{
			return true;
		}
		else if (line.compare(0, 5, "PART ") == 0)
		{
			return true;
		}
		else if (line.compare(0, 5, "MODE ") == 0)
		{
			return true;
		}
		else if (line.compare(0, 6, "TOPIC ") == 0)
		{
			return true;
		}
		else if (line.compare(0, 5, "LIST ") == 0)
		{
			return true;
		}
		else if (line.compare(0, 5, "KICK ") == 0)
		{
			return true;
		}
		else if (line.compare(0, 8, "PRIVMSG ") == 0)
		{
			return true;
		}
		else if (line.compare(0, 7, "NOTICE ") == 0)
		{
			return true;
		}
		return false;
}

void	Server::_send_txt(pollfd poll_fd, string text) const// couper en 512 et rajouter \r\n
{
	string prefixe;
	string dest;
	int p_size = 0;
	if (text[0] == ':')
		getNextWord(text, &p_size, prefixe);
	int i = p_size;
	while(text[i] == ' ' && text[i] != 0)
		i++;
	while(text[i] != 0)
	{
		dest = dest + prefixe;
		while (dest.size() < 510 && text[i] != 0)
		{
			dest = dest + text[i];
			i++;
		}
		dest = dest + '\r';
		dest = dest + '\n';
		send(poll_fd.fd, dest.c_str(), dest.size(), 0);
		dest.clear();
	}
}
