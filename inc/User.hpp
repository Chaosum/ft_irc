/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:46:36 by mservage          #+#    #+#             */
/*   Updated: 2022/05/30 18:29:32 by lgaudet-         ###   ########.fr       */
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
	string	_name;
	string	_nick;
	string	_password;
	pollfd		_poll_fd;
	bool		_isAuthentified;
	bool		_isOp;
public:
	User();
	User(string name, string password);
	User(User &src);

	~User();
	User	&operator=(User const &rhs);
	pollfd	getPollFd(void) const;
	string	getName(void) const;
	void	setName(string name);
	string	getPassword(void) const;
	void	setPassword(string password);
};
