/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:46:36 by mservage          #+#    #+#             */
/*   Updated: 2022/08/07 18:06:14 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <poll.h>

/* user contient:
	son nom
	son password
	le fd de sa socket
	*/

using namespace std;

class User
{
private:
	string	_userName;
	string	_realName;
	string	_hostName;
	string	_nick;
	string	_suppliedPassword;
	pollfd	_poll_fd;
	bool	_isAuth;
	string	_password;
	string	_poll_read;
public:
	User();
	User(pollfd newUser_pollfd);
	User(const User & src);
	User &operator=(User const &rhs);

	~User();

	pollfd	getPollFd(void) const;
	string	getUserName(void) const;
	void	setUserName(string name);
	string	getRealName(void) const;
	void	setRealName(string name);
	string	getHostName(void) const;
	void	setHostName(string name);
	string	getNick(void) const;
	void	setNick(string nick);
	string	getSuppliedPasswd(void) const;
	void	setPasswd(string passwd);
	bool	isAuth() const;
	bool	tryAuth(string serverPassword);
	string	getPollRead() const;
	void	incrPollRead(char *message);
	void	setPollRead(std::string message);
};
