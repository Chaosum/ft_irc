/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 11:01:01 by matthieu          #+#    #+#             */
/*   Updated: 2022/05/30 16:07:57 by lgaudet-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <vector>
#include "User.hpp"

class Channel
{
private:
	std::string			_name;
	std::string			_password;
	std::string			_topic;
	std::vector<User*>	_members;
	std::vector<User*>	_ops;
	std::vector<User*>	_invited;
	std::vector<User*>	_banned;
	std::vector<User*>	_muted;
	std::size_t			_maxNbOfUsers;
	bool				_isPrivate;
	bool				_isSecret;
	bool				_inviteOnly;
	bool				_topicSettableOnlyByOp;

	Channel();
	Channel(Channel const &src);
	bool isOp(User const * user);
	
public:
	Channel(std::string name);
	
	~Channel();
	Channel &operator=(Channel const &rhs);

	std::string getName();
	std::string getTopic();
	bool verifyPassword(std::string password);
	// Returns true on successful change
	bool setPassword(User const * user, std::string newPasswd);
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
	bool setTopic(User const * user, std::string topic);
	bool setName(User const * user, std::string name);
};
