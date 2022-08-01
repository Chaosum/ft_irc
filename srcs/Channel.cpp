/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 11:25:32 by matthieu          #+#    #+#             */
/*   Updated: 2022/08/01 15:48:50 by lgaudet-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel():_name(""),
				   _topic(""),
				   _members(vector<User*>()),
				   _chanOps(vector<User*>()),
				   _maxNbOfUsers(numeric_limits<int>::max()),
				   _isPrivate(false),
				   _isSecret(false),
				   _topicSettableOnlyByOp(false) {
}

Channel::Channel(const Channel & src) {
	this->_name = src._name;
	this->_topic = src._topic;
	this->_members = src._members;
	this->_chanOps = src._chanOps;
	this->_maxNbOfUsers = src._maxNbOfUsers;
	this->_isPrivate = src._isPrivate;
	this->_isSecret = src._isSecret;
	this->_topicSettableOnlyByOp = src._topicSettableOnlyByOp;
}

Channel::Channel(string name): _topic(""),
							   _members(vector<User*>()),
							   _chanOps(vector<User*>()),
							   _maxNbOfUsers(numeric_limits<int>::max()),
							   _isPrivate(false),
							   _isSecret(false),
							   _topicSettableOnlyByOp(false) {
	this->_name = name;
}
	
Channel::~Channel() {}

Channel & Channel::operator=(const Channel & rhs) {
	this->_name = rhs._name;
	this->_topic = rhs._topic;
	this->_members = rhs._members;
	this->_chanOps = rhs._chanOps;
	this->_maxNbOfUsers = rhs._maxNbOfUsers;
	this->_isPrivate = rhs._isPrivate;
	this->_isSecret = rhs._isSecret;
	this->_topicSettableOnlyByOp = rhs._topicSettableOnlyByOp;

	return *this;
}

string Channel::getName() const { return _name; }
string Channel::getTopic() const { return _topic; }
size_t Channel::getNumberOfMembers() const { return _members.size(); }
vector<User*> Channel::getMembers() const { return _members; }
size_t Channel::getMaxNbOfUser() const { return _maxNbOfUsers; }
bool Channel::isTopicSettableOnlyByOp() const { return _topicSettableOnlyByOp; }
bool Channel::isSecret() const { return _isSecret; }
bool Channel::isPrivate() const { return _isPrivate; }
bool Channel::canUserMessageChannel(User const * user) const {  // see ERR_CANNOTSENDTOCHAN (404)
	(void)user;
	return true;
}

User * Channel::getMember(string nick) const {
	vector<User*>::const_iterator it;
	for (it = _members.begin() ; it != _members.end() ; ++it)
		if ((*it)->getNick() == nick)
			break ;
	if ((*it)->getNick() == nick)
		return *it;
	return NULL;
}

bool Channel::isUserInChannel(User const * user) const {
	vector<User*>::const_iterator it;
	for (it = _members.begin() ; it != _members.end() ; ++it)
		if (*it == user)
			break ;
	if (*it == user)
		return true;
	return false;
}

bool Channel::isUserChanOp(User const * user) const {
	vector<User*>::const_iterator it;
	for (it = _chanOps.begin() ; it != _chanOps.end() ; ++it)
		if (*it == user)
			break ;
	if (*it == user)
		return true;
	return false;
}

void Channel::setUserChanOp(User * user, bool value) {
	vector<User*>::iterator it;

	// On vérifie que l'utilisateur est dans le channel avant de le mettre chanop
	for (it = _members.begin() ; it != _members.end() ; ++it)
		if (*it == user)
			break ;
	if (it == _members.end())
		return ;

	if (value)
		_chanOps.push_back(user);
	else {
		for (it = _chanOps.begin() ; it != _chanOps.end() ; ++it)
			if (*it == user)
				break ;
		if (*it == user)
			_chanOps.erase(it);
	}
}

bool Channel::addUser(User * user) {
	vector<User*>::const_iterator it;

	for (it = _members.begin() ; it != _members.end() ; ++it)
		if (*it == user)
			return false;
	_members.push_back(user);
	return true;
}

bool Channel::deleteUserFromChannel(User * user) {
	vector<User*>::iterator it;

	for (it = _members.begin() ; it != _members.end() ; ++it)
		if (*it == user)
			break ;
	if (it == _members.end())
		return false;
	_members.erase(it);
	return true;
}

bool Channel::deleteUserFromChannel(string nickname) {
	vector<User*>::iterator it;

	for (it = _members.begin() ; it != _members.end() ; ++it)
		if ((*it)->getNick() == nickname)
			break ;
	if (it == _members.end())
		return false;
	_members.erase(it);
	return true;
}

bool Channel::setPrivate(User const * user, bool value) {
	vector<User*>::const_iterator it;

	for (it = _members.begin() ; it != _members.end() ; ++it)
		if (*it == user)
			break ;
	if (it == _members.end())
		return false;
	_isPrivate = value;
	return true;
}

bool Channel::setSecret(User const * user, bool value) {
	vector<User*>::const_iterator it;

	for (it = _members.begin() ; it != _members.end() ; ++it)
		if (*it == user)
			break ;
	if (it == _members.end())
		return false;
	_isSecret = value;
	return true;
}

bool Channel::setTopicSettableOnlyByOp(User const * user, bool value) {
	vector<User*>::const_iterator it;

	for (it = _members.begin() ; it != _members.end() ; ++it)
		if (*it == user)
			break ;
	if (it == _members.end())
		return false;
	_topicSettableOnlyByOp = value;
	return true;
}

bool Channel::setMaxNbOfUsers(User const * op, int maxNb) {
	vector<User*>::const_iterator it;

	for (it = _members.begin() ; it != _members.end() ; ++it)
		if (*it == op)
			break ;
	if (it == _members.end())
		return false;
	_maxNbOfUsers = maxNb;
	return true;
}

bool Channel::setTopic(User const * user, string topic) {
	vector<User*>::const_iterator it;

	for (it = _members.begin() ; it != _members.end() ; ++it)
		if (*it == user)
			break ;
	if (it == _members.end())
		return false;
	_topic = topic;
	return true;
}

bool Channel::setName(User const * user, string name) {
	vector<User*>::const_iterator it;

	for (it = _members.begin() ; it != _members.end() ; ++it)
		if (*it == user)
			break ;
	if (it == _members.end())
		return false;
	_name = name;
	return true;
}
