/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mservage <mservage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/20 15:46:36 by mservage          #+#    #+#             */
/*   Updated: 2022/04/22 17:58:19 by mservage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>

class User
{
private:
	std::string	_name;
	std::string	_password;
	int			_fd;
	int			_network_soket;
public:
	User();
	User(std::string name);
	User(User &src);

	~User();
	User	&operator=(User const &rhs);
	int	getFd(void) const;
	std::string	getName(void) const;
	void	setName(std::string name);
	std::string	getPassword(void) const;
	void	setPassword(std::string password);
};
