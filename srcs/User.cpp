/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 17:19:13 by mservage          #+#    #+#             */
/*   Updated: 2022/07/14 13:10:33 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

User::User()
{
}

User::User(pollfd newUser_pollfd)
{
	_isAuth = 0;
	_isOp = 0;
	_nick = "";
	_poll_fd.events = newUser_pollfd.events;
	_poll_fd.revents = newUser_pollfd.revents;
	_poll_fd.fd = newUser_pollfd.fd;
}

User::User(User const &src)
{
	this->_userName = src._userName;//a refaire
	this->_poll_fd = src._poll_fd;
}

User::~User()
{
}

User	&User::operator=(User const &rhs)// arefaire
{
	this->_userName = rhs._userName;
	this->_poll_fd = rhs._poll_fd;

	return (*this);
}

bool	User::isOp()
{
	return (this->_isOp);
}

pollfd	User::getPollFd(void) const
{
	return (this->_poll_fd);
}

std::string	User::getUserName(void) const
{
	return (this->_userName);
}

void	User::setUserName(std::string name)
{
	this->_userName = name;
}

std::string	User::getRealName(void) const
{
	return (this->_userName);
}

void	User::setRealName(string name)
{
	_realName = name;
}

string	User::getHostName(void) const
{
	return (this->_hostName);
}

void	User::setHostName(string name)
{
	_hostName = name;
}

string	User::getNick(void) const
{
	return (this->_nick);
}

void	User::setNick(string nick)
{
	_nick = nick;
}

string	User::getSuppliedPasswd(void) const
{
	return (this->_suppliedPassword);
}

void	User::setPasswd(string passwd)
{
	_password = passwd;
}

bool	User::isAuth() const
{
	return (this->_isAuth);
}

bool	User::tryAuth()
{
	if (isAuth())
		return (true);
	return (false);
}