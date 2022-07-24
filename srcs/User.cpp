/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 17:19:13 by mservage          #+#    #+#             */
/*   Updated: 2022/07/24 18:54:11 by lgaudet-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

User::User()
{
}

User::User(pollfd newUser_pollfd): _userName(""), 
								   _realName(""), 
								   _nick(""),
								   _suppliedPassword(""), 
								   _isAuth(false) {
	_poll_fd.events = newUser_pollfd.events;
	_poll_fd.revents = newUser_pollfd.revents;
	_poll_fd.fd = newUser_pollfd.fd;
}

User::User(const User & src): _userName(""), 
                       _realName(""), 
                       _nick(""),
                       _suppliedPassword(""), 
					   _poll_fd(), 
					   _isAuth(false) {
	_poll_fd.events = src._poll_fd.events;
	_poll_fd.revents = src._poll_fd.revents;
	_poll_fd.fd = src._poll_fd.fd;
}

User::~User() {
}

User	&User::operator=(const User &rhs) {
	this->_userName = rhs._userName;
	this->_realName = rhs._realName;
	this->_nick = rhs._nick;
	this->_suppliedPassword = rhs._suppliedPassword;
	this->_poll_fd = rhs._poll_fd;
	this->_poll_fd.events = rhs._poll_fd.events;
	this->_poll_fd.revents = rhs._poll_fd.revents;
	this->_poll_fd.fd = rhs._poll_fd.fd;
	this->_isAuth = rhs._isAuth;
	return (*this);
}

pollfd	User::getPollFd(void) const
{
	return (this->_poll_fd);
}

string User::getUserName() const { return _userName; }
void User::setUserName(string name) { _userName = name; }
string User::getRealName() const { return _realName; }
void User::setRealName(string name) { _realName = name; }
string User::getHostName(void) const { return _hostName; }
void User::setHostName(string name) { _hostName = name; }
string User::getNick() const { return _nick; }
void User::setNick(string nick) { _nick = nick; }
string User::getSuppliedPasswd() const { return _suppliedPassword; }
void User::setPasswd(string passwd) { _suppliedPassword = passwd; }
bool User::isAuth() const { return _isAuth; }
void User::tryAuth(string serverPassword) {
	if (!_userName.empty() && !_realName.empty() && !_nick.empty() && serverPassword == _suppliedPassword)
		_isAuth = true;
}
