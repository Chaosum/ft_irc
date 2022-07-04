/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 11:01:01 by matthieu          #+#    #+#             */
/*   Updated: 2022/07/04 17:33:27 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "User.hpp"

using namespace std;

class Channel
{
private:
	string			_name;
	string			_topic;
	vector<User*>	_members;
	vector<User*>	_ops;
	size_t			_maxNbOfUsers;
	bool			_isPrivate;
	bool			_isSecret;
	bool			_topicSettableOnlyByOp;

	Channel();
	Channel(Channel const &src);
	bool isOp(User const * user);
	
public:
	Channel(string name);
	
	~Channel();
	Channel &operator=(Channel const &rhs);

	string getName();
	string getTopic();
	size_t getNumberOfMembers();
	const vector<User*> getMembers();
	bool canUserMessageChannel(User * user); // see ERR_CANNOTSENDTOCHAN (404)
	bool isUserInChannel(User * user);
	bool addUser(User * user);
	void deleteUserFromChannel(User * user);
	// Returns true on successful change
	bool setPrivate(User const * user, bool value);
	bool setSecret(User const * user, bool value);
	bool setTopicSettableOnlyByOp(User const * user, bool value);
	bool setOperator(User const * op, User const * newOp);
	bool setMaxNbOfUsers(User const * op, int maxNb);
	bool setTopic(User const * user, string topic);
	bool setName(User const * user, string name);
};
