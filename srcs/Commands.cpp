/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudet- <lgaudet-@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 22:01:07 by lgaudet-          #+#    #+#             */
/*   Updated: 2022/06/07 22:20:39 by lgaudet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

string Server::_composePrefix(User * sender) {
	string res = "";

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
}
string Server::join(User * user, vector<string> & requested_channels, vector<string> & passwords) {
}
string Server::part(User * user, vector<string> & channels) {
}
string Server::mode(User * user, string requested_channel, vector<string> & operands) {
}
string Server::topic(User * user, string channel, string topic) {
}
string Server::list(User * user, vector<string> & channels) {
}
string Server::invite(User * user, string nick, string channel) {
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
	for (it = _users.begin() ; it != _users.end() ; ++it) {
		if (it->getNick() == recipient) {
			_send_txt(it->getPollFd(), _composePrefix(user) + "NOTICE " + recipient + " :" + msg);
			break;
		}
	}
}
