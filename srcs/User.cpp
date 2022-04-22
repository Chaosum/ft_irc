/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mservage <mservage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 17:19:13 by mservage          #+#    #+#             */
/*   Updated: 2022/04/22 17:55:08 by mservage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/User.hpp"

User::User()
{
}

User::User(std::string name)
{
	this->_name= name;
}

User::User(User &src)
{
	this->_name = src._name;
	this->_password = src._password;
	this->_network_soket = src._network_soket;
	this->_fd = src._fd;
}

User::~User()
{
}

User	&User::operator=(User const &rhs)
{
	this->_name = rhs._name;
	this->_password = rhs._password;
	this->_network_soket = rhs._network_soket;
	this->_fd = rhs._fd;
	return (*this);
}

int	User::getFd(void) const
{
	return (this->_fd);
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
