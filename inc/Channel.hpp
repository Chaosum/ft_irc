/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 11:01:01 by matthieu          #+#    #+#             */
/*   Updated: 2022/04/28 11:48:05 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
