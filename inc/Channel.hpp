/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 11:01:01 by matthieu          #+#    #+#             */
/*   Updated: 2022/06/08 16:45:54 by lgaudet-         ###   ########.fr       */
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
	vector<User*>	_banned;
	vector<User*>	_muted;
	size_t			_maxNbOfUsers;
	bool			_isPrivate;
	bool			_isSecret;
	bool			_inviteOnly;
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
	const vector<User*> getMembers();
	bool verifyPassword(string password);
	bool canUserMessageChannel(User * user); // see ERR_CANNOTSENDTOCHAN (404)
	// Returns true on successful change
	bool setPassword(User const * user, string newPasswd);
	bool setPrivate(User const * user, bool value);
	bool setSecret(User const * user, bool value);
	bool setInvite(User const * user, bool value);
	bool setTopicSettableOnlyByOp(User const * user, bool value);
	bool setOperator(User const * op, User const * newOp);
	bool banUser(User const * op, User const * newOp);
	bool unBanUser(User const * op, User const * newOp);
	bool muteUser(User const * op, User const * newOp);
	bool unMuteUser(User const * op, User const * newOp);
	bool setMaxNbOfUsers(User const * op, int maxNb);
	bool invite(User const * op, User const * user);
	bool unInvite(User const * op, User const * user);
	bool setTopic(User const * user, string topic);
	bool setName(User const * user, string name);
};
