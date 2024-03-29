/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mservage <mservage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/28 11:25:32 by matthieu          #+#    #+#             */
/*   Updated: 2022/08/11 14:07:13 by mservage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Channel.hpp"

Channel::Channel():_name(""),
				   _topic(""),
				   _members(vector<string>()),
				   _chanOps(vector<string>()),
				   _maxNbOfUsers(0),
				   _isPrivate(false),
				   _isSecret(false),
				   _topicSettableOnlyByOp(true),
				   _isMessageFromOutsideAllowed(false) {
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
	this->_isMessageFromOutsideAllowed = src._isMessageFromOutsideAllowed;
}

Channel::Channel(string name): _name(name),
							   _topic(""),
							   _members(vector<string>()),
							   _chanOps(vector<string>()),
							   _maxNbOfUsers(0),
							   _isPrivate(false),
							   _isSecret(false),
							   _topicSettableOnlyByOp(true),
							   _isMessageFromOutsideAllowed(false) {
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
	this->_isMessageFromOutsideAllowed = rhs._isMessageFromOutsideAllowed;

	return *this;
}

string Channel::getName() const { return _name; }
string Channel::getTopic() const { return _topic; }
int Channel::getNumberOfMembers() const { return _maxNbOfUsers; }
vector<string>::const_iterator Channel::membersBegin() const { return _members.begin(); }
vector<string>::const_iterator Channel::membersEnd() const { return _members.end(); }
vector<string>::const_iterator Channel::opsBegin() const { return _chanOps.begin(); }
vector<string>::const_iterator Channel::opsEnd() const { return _chanOps.end(); }
int Channel::getMaxNbOfUser() const { return _maxNbOfUsers; }
bool Channel::isTopicSettableOnlyByOp() const { return _topicSettableOnlyByOp; }
bool Channel::isSecret() const { return _isSecret; }
bool Channel::isPrivate() const { return _isPrivate; }
bool Channel::isMessageFromOutsideAllowed() const { return _isMessageFromOutsideAllowed; }
bool Channel::setMessageFromOutsideAllowed(string nick, bool value) {
	vector<string>::const_iterator user;

	for (user = _chanOps.begin() ; user != _chanOps.end() ; ++user)
		if (*user == nick) {
			_isMessageFromOutsideAllowed = value;
			return true;
		}
	return false;
}

bool Channel::canUserMessageChannel(string nick) const {  // see ERR_CANNOTSENDTOCHAN (404)
	vector<string>::const_iterator user;

	if (_isMessageFromOutsideAllowed)
		return true;
	for (user = _members.begin() ; user != _members.end() ; ++user)
		if (*user == nick)
			return true;
	return false;
}

bool Channel::isUserInChannel(string nick) const {
	vector<string>::const_iterator it;
	for (it = _members.begin() ; it != _members.end() ; ++it)
		if (*it == nick)
			return true;
	return false;
}

bool Channel::isUserChanOp(string nick) const {
	vector<string>::const_iterator it;
	for (it = _chanOps.begin() ; it != _chanOps.end() ; ++it)
		if (*it == nick)
			return true;
	return false;
}

void Channel::setUserChanOp(string nick, bool value) {
	vector<string>::iterator it;

	// On vérifie que l'utilisateur est dans le channel avant de le mettre chanop
	for (it = _members.begin() ; it != _members.end() ; ++it)
		if (*it == nick)
			break ;
	if (it == _members.end())
		return ;

	if (value) { // On ajoute le nick à la liste s'il n'y est pas déjà
		for (it = _chanOps.begin() ; it != _chanOps.end() ; ++it)
			if (*it == nick)
				return ;
		_chanOps.push_back(nick);
	}
	else {
		for (it = _chanOps.begin() ; it != _chanOps.end() ; ++it)
			if (*it == nick)
				break ;
		if (*it == nick)
			_chanOps.erase(it);
	}
}

void Channel::changeNickOfUser(string oldNick, string newNick) {
	vector<string>::iterator user;

	for (user = _members.begin() ; user != _members.end() ; ++user)
		if (*user == oldNick) {
			*user = newNick;
			break ;
		}
	for (user = _chanOps.begin() ; user != _chanOps.end() ; ++user)
		if (*user == oldNick) {
			*user = newNick;
			break ;
		}
}

bool Channel::addUser(string nick) {
	vector<string>::const_iterator it;

	if (_maxNbOfUsers != 0 && _members.size() == (size_t)_maxNbOfUsers) // On vérifie si le channel est plein
		return false;
	for (it = _members.begin() ; it != _members.end() ; ++it)
		if (*it == nick)
			return false;
	_members.push_back(nick);
	return true;
}

bool Channel::deleteUserFromChannel(string nick) {
	vector<string>::iterator it;

	for (it = _members.begin() ; it != _members.end() ; ++it)
		if (*it == nick)
			break ;
	if (it == _members.end())
		return false;
	_members.erase(it);
	for (it = _chanOps.begin() ; it != _chanOps.end() ; ++it)
		if (*it == nick)
			break ;
	if (it != _chanOps.end())
		_chanOps.erase(it);
	return true;
}

bool Channel::setPrivate(string nick, bool value) {
	vector<string>::const_iterator it;

	for (it = _chanOps.begin() ; it != _chanOps.end() ; ++it)
		if (*it == nick)
			break ;
	if (it == _chanOps.end())
		return false;
	_isPrivate = value;
	return true;
}

bool Channel::setSecret(string nick, bool value) {
	vector<string>::const_iterator it;

	for (it = _chanOps.begin() ; it != _chanOps.end() ; ++it)
		if (*it == nick)
			break ;
	if (it == _members.end())
		return false;
	_isSecret = value;
	return true;
}

bool Channel::setTopicSettableOnlyByOp(string nick, bool value) {
	vector<string>::const_iterator it;

	for (it = _chanOps.begin() ; it != _chanOps.end() ; ++it)
		if (*it == nick)
			break ;
	if (it == _chanOps.end())
		return false;
	_topicSettableOnlyByOp = value;
	return true;
}

bool Channel::setMaxNbOfUsers(string nick, int maxNb) {
	vector<string>::const_iterator it;

	for (it = _chanOps.begin() ; it != _chanOps.end() ; ++it)
		if (*it == nick)
			break ;
	if (it == _chanOps.end())
		return false;
	_maxNbOfUsers = maxNb;
	return true;
}

bool Channel::setTopic(string nick, string topic) {
	vector<string>::const_iterator it;

	for (it = _members.begin() ; it != _members.end() ; ++it)
		if (*it == nick)
			break ;
	if (it == _members.end())
		return false;
	if (_topicSettableOnlyByOp) {
		for (it = _chanOps.begin() ; it != _chanOps.end() ; ++it)
			if (*it == nick)
				break ;
		if (it == _chanOps.end())
			return false;
	}
	_topic = topic;
	return true;
}

bool Channel::setName(string nick, string name) {
	vector<string>::const_iterator it;

	for (it = _chanOps.begin() ; it != _chanOps.end() ; ++it)
		if (*it == nick)
			break ;
	if (it == _chanOps.end())
		return false;
	_name = name;
	return true;
}
