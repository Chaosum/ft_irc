/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 11:01:01 by matthieu          #+#    #+#             */
/*   Updated: 2022/05/13 13:54:35 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>

class Channel
{
private:
	std::string	_name;

	Channel();
	Channel(Channel const &src);
	
public:
	Channel(std::string name);
	
	~Channel();
	Channel &operator=(Channel const &rhs);
};
