/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 17:19:13 by mservage          #+#    #+#             */
/*   Updated: 2022/04/29 13:52:42 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

User::User()
{
}

User::User(std::string name, std::string password): _name(name), _password(password), _isAuthentified(0)
{
	_poll_fd.fd = socket(AF_INET, SOCK_STREAM, 0);
	_poll_fd.events = POLLIN|POLLOUT;
	_poll_fd.revents = 0;
}

User::User(User &src)
{
	this->_name = src._name;
	this->_password = src._password;
	this->_poll_fd = src._poll_fd;
}

User::~User()
{
}

User	&User::operator=(User const &rhs)
{
	this->_name = rhs._name;
	this->_password = rhs._password;
	this->_poll_fd = rhs._poll_fd;

	return (*this);
}

pollfd	User::getPollFd(void) const
{
	return (this->_poll_fd);
}

std::string	User::getName(void) const
{
	return (this->_name);
}

void	User::setName(std::string name)
{
	this->_name = name;
}

std::string	User::getPassword(void) const
{
	return (this->_password);
}

void	User::setPassword(std::string password)
{
	this->_password = password;
}
