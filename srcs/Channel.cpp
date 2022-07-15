/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 11:25:32 by matthieu          #+#    #+#             */
/*   Updated: 2022/07/14 13:25:10 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

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

string			Channel::getName() const
{
	return (this->_name);
}

string			Channel::getTopic() const
{
	return (this->_topic);
}

size_t			Channel::getNumberOfMembers() const
{
	return (this->_members.size());
}

vector<User*>	Channel::getMembers() const
{
	return (this->_members);
}

bool			Channel::canUserMessageChannel(User const * user) const // see ERR_CANNOTSENDTOCHAN (404)
{
	return (false);
}

bool			Channel::isUserInChannel(User const * user) const
{
	return (false);

}

bool			Channel::isUserOp(User const * user) const
{
	return (false);

}

void			Channel::makeUserOp(User * user) // Returns ton successful change
{


}

bool			Channel::addUser(User const * user)
{
	return (false);

}

bool			Channel::deleteUserFromChannel(User * user)
{
	return (false);

}

bool			Channel::deleteUserFromChannel(string nickname)
{
	return (false);

}

bool			Channel::setPrivate(User const * user, bool value)
{
	return (false);
	
}

bool			Channel::isPrivate() const
{
	return (false);

}

bool			Channel::setSecret(User const * user, bool value)
{
	return (false);

}

bool			Channel::isSecret() const
{
	return (false);

}

bool			Channel::setTopicSettableOnlyByOp(User const * user, bool value)
{
	return (false);

}

bool			Channel::setOperator(User const * op, User const * newOp)
{
	return (false);

}

bool			Channel::setMaxNbOfUsers(User const * op, int maxNb)
{
	return (false);
	
}

bool			Channel::setTopic(User const * user, string topic)
{
	return (false);

}

bool			Channel::setName(User const * user, string name)
{
	return (false);
	
}