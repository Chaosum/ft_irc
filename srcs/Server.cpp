/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mservage <mservage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 17:34:40 by matthieu          #+#    #+#             */
/*   Updated: 2022/08/10 17:51:01 by mservage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netinet/in.h>
#include <unistd.h>
#include "../inc/Server.hpp"

#define MAXUSER 666

Server::Server(): _password("blabla"), _port(0)
{
}

Server::Server(Server const &src)
{
	this->_server_name = src._server_name;
	this->_port = src._port;
	this->_password = src._password;
}

Server::Server(std::string server_name, int port, char *password): _server_name(server_name), _password(password), _port(port)
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
	fcntl(poll_fd.fd, F_SETFL, O_NONBLOCK);
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
			fcntl(new_user, F_SETFL, O_NONBLOCK);
			newUser_pollfd.fd = new_user;
			newUser_pollfd.events = POLLIN|POLLOUT;
			newUser_pollfd.revents = 0;
			_fds.push_back(newUser_pollfd);
			User newUser(newUser_pollfd);
			_users.push_back(newUser);
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
				_users[index].incrPollRead(buf);
				printf("%s", buf);
				if (read_ret == 0)
				{
					printf("fd = %d et revent = %d\n", it->fd, it->revents);
					quit(&_users[index], "");
					close(it->fd);
					it = _fds.erase(it); 
					_users.erase( _users.begin() + index);
					it = _fds.begin() + 1;
					index = 0;
					continue;
				}
				else if (int valid_command = is_command(_users[index].getPollRead()))
				{
					if (msg_parse(_users[index].getPollRead().substr(0, valid_command), index) == 1)
					{
						printf("fd = %d et revent = %d\n", it->fd, it->revents);
						close(it->fd);
						it = _fds.erase(it); 
						_users.erase( _users.begin() + index);
						it = _fds.begin() + 1;
						index = 0;
						continue ;
					}
					_users[index].setPollRead(_users[index].getPollRead().substr(valid_command + 1, _users[index].getPollRead().size() - (valid_command + 1)));
				}
			}
			index++;
			it++;
		}
		_fds[0].revents = 0;
	}
}

int	Server::is_command(std::string command)
{
	int i = 0;
	while (command[i] != 0)
	{
		if (command[i] == '\r' && command[i + 1] == '\n')
			return (i + 1);
		i++;
	}
	return (0);
}

std::string	Server::getNextWord(std::string line, int *i) const
{
	std::string tmp;
	while (line[*i] == ' ')
	{
		*i = *i + 1;
	}
	if (line[*i] == ':')
	{
		*i = *i + 1;
		while (line[*i] != '\r' && line[*i] != 0)
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

std::vector<std::string>	Server::getNextVector(std::string line, int *i)
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
	return (dest);
}

std::vector<std::string>	Server::getNextVectorcomma(std::string line, int *i)
{
	std::vector<std::string> dest;
	std::string tmp;
	int i_save = 0;
	while (line[*i] != '\r' && line[*i] != 0)
	{
		tmp.clear();
		i_save = *i;
		while (line[*i] == ' ' || line[*i] == ',')
			*i = *i + 1;
		if (line[*i] == ':')
			return (dest);
		while (line[*i] != ' ' && line[*i] != ',' && line[*i] != 0 && line[*i] != '\r')
		{
			tmp = tmp + line[*i];
			*i = *i + 1;
		}
		dest.push_back(tmp);
	}
	return (dest);
}

int	Server::msg_parse(std::string buf, int index)
{
	size_t i = 0;
	std::string	line;
	std::vector<string> temp_vector;
	std::string command;
	while (i < buf.size())
	{
		int tmp_i = 0;
		while (buf[i] != 0 && buf[i] != '\r')
		{
			line = line + buf[i];
			i++;
		}
		if (line[0] == ':')
		{
			getNextWord(line, &(tmp_i = 1));
			command = getNextWord(line, &tmp_i);
		}
		else
		{
			command = getNextWord(line, &tmp_i);
		}
		if (command == "PASS")
		{
			pass(&_users[index], getNextWord(line, &tmp_i));
		}
		else if (command == "NICK")
		{
			nick(&_users[index], getNextWord(line, &tmp_i));
		}
		else if (command == "USER") // :
		{
			std::string userName = getNextWord(line, &tmp_i);
			std::string hostName = getNextWord(line, &tmp_i);
			std::string serverName = getNextWord(line, &tmp_i);
			std::string realName = getNextWord(line, &tmp_i);
			user(&_users[index], userName, hostName, serverName, realName);
		}
		else if (!_command_exists(command))
			unknownCommand(&_users[index], command);
		else if (!_users[index].isAuth())
			notLoggedIn(&_users[index]);
		else if (command == "QUIT")
		{
			quit(&_users[index], getNextWord(line, &tmp_i));
			return (1);
		}
		else if (command == "JOIN")
		{
			join(&_users[index], temp_vector = getNextVectorcomma(line, &tmp_i)); 
		}
		else if (command == "PING")
		{
			pong(&_users[index], getNextWord(line, &tmp_i));
		}
		else if (command == "PART")
		{
			temp_vector = getNextVectorcomma(line, &tmp_i);
			std::string part_msg = getNextWord(line, &tmp_i);
			part(&_users[index], temp_vector, part_msg);
		}
		else if (command == "MODE")
		{
			std::string requested_channel_or_user = getNextWord(line, &tmp_i);
			mode(&_users[index], requested_channel_or_user, temp_vector = getNextVector(line, &tmp_i));
		}
		else if (command == "TOPIC") // :
		{
			std::string t_chanel = getNextWord(line, &tmp_i);
			std::string t_topic = getNextWord(line, &tmp_i);
			topic(&_users[index], t_chanel, t_topic);
		}
		else if (command == "LIST")
		{
			list(&_users[index], temp_vector = getNextVectorcomma(line, &tmp_i));
		}
		else if (command == "KICK") // :
		{
			std::string k_channel = getNextWord(line, &tmp_i);
			std::string k_keckee = getNextWord(line, &tmp_i);
			std::string k_comment = getNextWord(line, &tmp_i);
			kick(&_users[index], k_channel, k_keckee, k_comment);
		}
		else if (command == "PRIVMSG") // :
		{
			temp_vector = getNextVectorcomma(line, &tmp_i);
			std::string priv_msg = getNextWord(line, &tmp_i);
			privmsg(&_users[index], temp_vector, priv_msg);
		}
		else if (command == "NOTICE")
		{
			temp_vector = getNextVectorcomma(line, &tmp_i);
			std::string n_msg = getNextWord(line, &tmp_i);
			notice(&_users[index], temp_vector, n_msg);
		}
		i = i + 2;
		line.erase();
	}
	return (0);
}

bool	Server::_command_exists(std::string command)
{
		if (command == "QUIT")
		{
			return true;
		}
		else if (command == "JOIN")
		{
			return true;
		}		
		else if (command == "PING")
		{
			return true;
		}
		else if (command == "PART")
		{
			return true;
		}
		else if (command == "MODE")
		{
			return true;
		}
		else if (command == "TOPIC")
		{
			return true;
		}
		else if (command == "LIST")
		{
			return true;
		}
		else if (command == "KICK")
		{
			return true;
		}
		else if (command == "PRIVMSG")
		{
			return true;
		}
		else if (command == "NOTICE")
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
	if (text[0] == ':') {
		p_size = 1;
		prefixe = getNextWord(text, &p_size);
		prefixe = ":" + prefixe;
	}
	int i = p_size;
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
