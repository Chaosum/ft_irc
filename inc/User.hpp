/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:46:36 by mservage          #+#    #+#             */
/*   Updated: 2022/05/20 16:45:07 by lgaudet-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <poll.h>

/* user contient:
	son nom
	son password
	le fd de sa socket
	*/

class User
{
private:
	std::string	_name;
	std::string	_password;
	pollfd		_poll_fd;
	bool		_isAuthentified;
	bool		_isOp;
public:
	User();
	User(std::string name, std::string password);
	User(User &src);

	~User();
	User	&operator=(User const &rhs);
	pollfd	getPollFd(void) const;
	std::string	getName(void) const;
	void	setName(std::string name);
	std::string	getPassword(void) const;
	void	setPassword(std::string password);
};
