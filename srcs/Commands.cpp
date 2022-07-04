/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudet- <lgaudet-@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 22:01:07 by lgaudet-          #+#    #+#             */
/*   Updated: 2022/06/20 13:51:32 by lgaudet-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

string Server::_composePrefix(User * sender) {
	string res = "";

	if (sender == NULL)
		return ":" + _server_name + " ";
	res.append(":" + sender->getNick() + "!" + sender->getName());
	res.append("@" + _server_name + " ");
	return res;
}

string Server::_sendPrivmsgToChan(User * sender, string channel, string text) {
	vector<Channel>::iterator chan;
	vector<User*>::const_iterator it;
	for (chan = _channels.begin() ; chan != _channels.end() ; ++chan)
		if (chan->getName() == channel)
			break;
	if (chan == _channels.end())
		return ":" + _server_name + " 401" + channel + " :No such nick/channel";
	if (chan->canUserMessageChannel(sender)) {
		for (it = chan->getMembers().begin() ; it != chan->getMembers().end() ; ++it)
			_send_txt((*it)->getPollFd(), _composePrefix(sender) + "PRIVMSG " + channel + " :" + text);
		return "";
	}
	else
		return ":" + _server_name + " 404" + channel + " :Cannot send to channel";
}

string Server::_sendPrivmsgToUser(User * sender, string recipient, string text) {
	vector<User>::iterator it;
	for (it = _users.begin() ; it != _users.end() ; ++it)
		if (it->getNick() == recipient)
			break; // the correct User is found
	if (it == _users.end())
		return ":" + _server_name + " 401" + recipient + " :No such nick/channel";
	_send_txt(it->getPollFd(), _composePrefix(sender) + "PRIVMSG" + recipient + " :" + text);
}

string Server::pass(User * user, string password) {
}
string Server::nick(User * user, string nickname) {
}
string Server::user(User * user, string username, string hostname, string servername, string realname) {
}
void Server::quit(User * user, string msg) {
	vector<Channel>::iterator chan;

	msg = msg == ""? user->getNick() + " is quitting the server" : msg;
	for (chan = _channels.begin() ; chan != _channels.end() ; ++chan)
		if (chan->isUserInChannel(user)) {
			notice(user, chan->getName(), msg);
			chan->deleteUserFromChannel(user);
		}
}

string Server::join(User * user, vector<string> & requested_channels) {
	vector<Channel>::iterator chan;
	vector<string>::iterator it;
	string topic;

	if (requested_channels.empty())
		_send_txt(user->getPollFd(), ":" + _server_name + " 461 JOIN :Not enough parameters");
	for (it = requested_channels.begin() ; it != requested_channels.end() ; ++it) {
		for (chan = _channels.begin() ; chan != _channels.end() ; ++chan)
			if (chan->getName() == *it)
				break;
		if (chan != _channels.end()) {
			chan->addUser(user);
			topic = chan->getTopic();
			if (topic == "")
				_send_txt(user->getPollFd(), _composePrefix(NULL) + "331 " + chan->getName() + " :No topic is set");
			else
				_send_txt(user->getPollFd(), _composePrefix(NULL) + "332 " + chan->getName() + " :" + topic);
		}
		else {
		// create channel
		}
	}
}

string Server::part(User * user, vector<string> & channels) {
}
string Server::mode(User * user, string requested_channel, vector<string> & operands) {
}
void Server::topic(User * user, string channel, string topic) {
	if (channel.empty()) {
		_send_txt(user->getPollFd(), _composePrefix(NULL) + "461 TOPIC :Not enough parameters");
		return ;
	}
	vector<Channel>::iterator it;
	for (it = _channels.begin() ; it != _channels.end() ; ++it) {
		if (it->getName() == channel)
			break ;
	}
	if (it == _channels.end() || !it->isUserInChannel(user)) {
		_send_txt(user->getPollFd(), _composePrefix(NULL) + "442 " + channel + " :You're not on that channel");
		return;
	}
	if (topic.empty()) {
		if (it->getTopic().empty())
			_send_txt(user->getPollFd(), _composePrefix(NULL) + "331 " + channel + " :No topic is set");
		else
			_send_txt(user->getPollFd(), _composePrefix(NULL) + "332 " + channel + " :" + it->getTopic()); 
	}
	else if (it->setTopic(user, topic))
		_send_txt(user->getPollFd(), _composePrefix(NULL) + "332 " + channel + " :" + it->getTopic());
	else
		_send_txt(user->getPollFd(), _composePrefix(NULL) + "482 " + channel + " :You're not channel operator");
}

string Server::kick(User * user, string channel, string kickee, string comment) {
}

void Server::privmsg(User * user, vector<string> & recipients, string msg) {
	if (recipients.empty()) {
		_send_txt(user->getPollFd(), "411 :No recipient given (<PRIVMSG>)");
		return ;
	}
	if (msg.empty()) {
		_send_txt(user->getPollFd(), "412 :No text to send");
		return ;
	}
	string res = "";
	vector<string>::iterator it;
	for (it = recipients.begin() ; it != recipients.end() ; ++it) {
		if ((*it)[0] == '#' || (*it)[0] == '&') // Case where the recipient is a channel
			res = _sendPrivmsgToChan(user, *it, msg);
		else // Case where the recipient is a user
			res = _sendPrivmsgToUser(user, *it, msg);
		if (res != "")
			_send_txt(user->getPollFd(), res);
	}
}

void Server::notice(User * user, string recipient, string msg) {
	vector<User>::iterator it;
	vector<User*>::const_iterator chan_user;
	vector<Channel>::iterator chan;

	if (recipient[0] == '#' || recipient[0] == '&') { // Case where the recipient is a channel
		for (chan = _channels.begin() ; chan != _channels.end() ; ++chan)
			if (chan->getName() == recipient)
				break;
		for (chan_user = chan->getMembers().begin() ; chan_user != chan->getMembers().end() ; ++chan_user) {
			_send_txt((*chan_user)->getPollFd(), _composePrefix(user) + "NOTICE " + (*chan_user)->getNick() + " :" + msg);
		}
	}
	else
		for (it = _users.begin() ; it != _users.end() ; ++it) {
			if (it->getNick() == recipient) {
				_send_txt(it->getPollFd(), _composePrefix(user) + "NOTICE " + recipient + " :" + msg);
				break;
			}
		}
}
