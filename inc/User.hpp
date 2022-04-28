/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:46:36 by mservage          #+#    #+#             */
/*   Updated: 2022/04/28 19:04:34 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>

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
	int			_fd;
public:
	User();
	User(std::string name, std::string password);
	User(User &src);

	~User();
	User	&operator=(User const &rhs);
	int	getFd(void) const;
	std::string	getName(void) const;
	void	setName(std::string name);
	std::string	getPassword(void) const;
	void	setPassword(std::string password);
};
