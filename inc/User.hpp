/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:46:36 by mservage          #+#    #+#             */
/*   Updated: 2022/07/04 17:23:09 by lgaudet-         ###   ########lyon.fr   */
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
	pollfd	_poll_fd;
	bool	_isOp;
public:
	User();
	User(string name, string password);
	User(User &src);

	~User();
	User	&operator=(User const &rhs);
	pollfd	getPollFd(void) const;
	string	getName(void) const;
	void	setName(string name);
	string	getNick(void) const;
	void	setNick(string nick);
};
