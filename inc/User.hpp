/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:46:36 by mservage          #+#    #+#             */
/*   Updated: 2022/07/05 19:09:31 by lgaudet-         ###   ########lyon.fr   */
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
	bool	_isOp;
	bool	_isAuth;
public:
	User();
	User(string name, string password);
	User(User &src);

	~User();

	string	getNick();
	bool	isOp();
	User	&operator=(User const &rhs);
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
	bool	tryAuth();
};
