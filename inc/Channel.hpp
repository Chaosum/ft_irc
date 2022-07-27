/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 11:01:01 by matthieu          #+#    #+#             */
/*   Updated: 2022/07/27 14:11:35 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <limits>
#include "User.hpp"

using namespace std;

class Channel
{
private:
	string			_name;
	string			_topic;
	vector<User*>	_members;
	vector<User*>	_chanOps;
	size_t			_maxNbOfUsers;
	bool			_isPrivate;
	bool			_isSecret;
	bool			_topicSettableOnlyByOp;

	
public:
	Channel();
	Channel(string name);
	Channel(const Channel & src);
	
	~Channel();
	Channel &operator=(const Channel & rhs);

	string			getName() const;
	string			getTopic() const;
	size_t			getNumberOfMembers() const;
	vector<User*>	getMembers() const;
	User *			getMember(string nick) const;
	size_t			getMaxNbOfUser() const;
	bool			canUserMessageChannel(User const * user) const; // see ERR_CANNOTSENDTOCHAN (404)
	bool			isUserInChannel(User const * user) const;
	bool			isUserChanOp(User const * user) const;
	bool			isTopicSettableOnlyByOp() const;
	void			setUserChanOp(User * user, bool value);
	// Returns ton successful change
	bool			addUser(User * user);
	bool			deleteUserFromChannel(User * user);
	bool			deleteUserFromChannel(string nickname);
	bool			setPrivate(User const * user, bool value);
	bool			isPrivate() const;
	bool			setSecret(User const * user, bool value);
	bool			isSecret() const;
	bool			setTopicSettableOnlyByOp(User const * user, bool value);
	bool			setMaxNbOfUsers(User const * op, int maxNb);
	bool			setTopic(User const * user, string topic);
	bool			setName(User const * user, string name);
};
