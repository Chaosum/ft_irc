/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 11:01:01 by matthieu          #+#    #+#             */
/*   Updated: 2022/08/08 17:01:39 by lgaudet-         ###   ########.fr       */
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
	string							_name;
	string							_topic;
	vector<string>					_members;
	vector<string>					_chanOps;
	int								_maxNbOfUsers;
	bool							_isPrivate;
	bool							_isSecret;
	bool							_topicSettableOnlyByOp;

	
public:
	Channel();
	Channel(string name);
	Channel(const Channel & src);
	
	~Channel();
	Channel &operator=(const Channel & rhs);

	string							getName() const;
	string							getTopic() const;
	int								getNumberOfMembers() const;
	vector<string>::const_iterator	membersBegin() const;
	vector<string>::const_iterator	membersEnd() const;
	vector<string>::const_iterator	opsBegin() const;
	vector<string>::const_iterator	opsEnd() const;
	int								getMaxNbOfUser() const;
	bool							canUserMessageChannel(string nick) const; // see ERR_CANNOTSENDTOCHAN (404)
	bool							isUserInChannel(string nick) const;
	bool							isUserChanOp(string nick) const;
	bool							isTopicSettableOnlyByOp() const;
	void							setUserChanOp(string nick, bool value);
	void							changeNickOfUser(string oldNick, string newNick);
	// Returns ton successful change
	bool							addUser(string nick);
	bool							deleteUserFromChannel(string nick);
	bool							setPrivate(string nick, bool value);
	bool							isPrivate() const;
	bool							setSecret(string nick, bool value);
	bool							isSecret() const;
	bool							setTopicSettableOnlyByOp(string nick, bool value);
	bool							setMaxNbOfUsers(string nick, int maxNb);
	bool							setTopic(string nick, string topic);
	bool							setName(string nick, string name);
};
