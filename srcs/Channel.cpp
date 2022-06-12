/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 11:25:32 by matthieu          #+#    #+#             */
/*   Updated: 2022/05/13 13:51:33 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel()
{
}

Channel::Channel(Channel const &src)
{
	this->_name = src._name;
}

Channel::Channel(std::string _name)
{
	this->_name = _name;
}

Channel::~Channel()
{
}

Channel &Channel::operator=(Channel const &rhs)
{
	this->_name = rhs._name;
	return (*this);
}