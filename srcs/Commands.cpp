/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgaudet- <lgaudet-@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 22:01:07 by lgaudet-          #+#    #+#             */
/*   Updated: 2022/07/08 17:01:26 by lgaudet-         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

// send the proper prefix of a message sent to a user
// if sender param is NULL, the sender in the prefix is actually the server
string Server::_composePrefix(User const * sender) const{
	string res = "";

	if (sender == NULL)
		return ":" + _server_name + " ";
	res.append(":" + sender->getNick() + "!" + sender->getUserName());
	res.append("@" + _server_name + " ");
	return res;
}

void Server::_sendPrivmsgToChan(User const * sender, string channel, string text) const{
	vector<Channel>::const_iterator chan;
	vector<User*>::const_iterator it;
	for (chan = _channels.begin() ; chan != _channels.end() ; ++chan)
		if (chan->getName() == channel)
			break;
	if (chan == _channels.end())
		_sendTextToUser(NULL, sender, "401" + channel + " :No such nick/channel");
	if (!chan->canUserMessageChannel(sender))
		_sendTextToUser(NULL, sender, "404 " + channel + " :Cannot send to channel");
	else
		_sendTextToChan(sender, *chan, _composePrefix(sender) + "PRIVMSG " + channel + " :" + text);
}

void Server::_sendPrivmsgToUser(User const * sender, string recipient, string text) const{
	vector<User>::const_iterator it;
	for (it = _users.begin() ; it != _users.end() ; ++it)
		if (it->getNick() == recipient)
			break; // the correct User is found
	if (it == _users.end())
		_sendTextToUser(NULL, sender, "401 " + recipient + " :No such nick/channel");
	_sendTextToUser(sender, &*it, _composePrefix(sender) + "PRIVMSG" + recipient + " :" + text);
}

void Server::_sendTextToChan(User const * sender, Channel const & chan, string text) const{
	vector<User*>::const_iterator it;

	for (it = chan.getMembers().begin() ; it != chan.getMembers().end() ; ++it) {
			_send_txt((*it)->getPollFd(), text);
		}
}

void Server::_sendTextToUser(User const * sender, User const * recipient, string text) const{
	_send_txt(recipient->getPollFd(), _composePrefix(sender) + text);
}

void	Server::_nameList(Channel const & chan, User const * recipient) const {
	vector<User*>::const_iterator member;
	for (member = chan.getMembers().begin() ; member != chan.getMembers().end() ; member++) {
		bool isOp = chan.isUserOp(*member) || (*member)->isOp();
		_sendTextToUser(NULL, recipient, "353 " + chan.getName() + " :" + (isOp?"@":"+") + (*member)->getNick());
	}
	_sendTextToUser(NULL, recipient, "366 " + chan.getName() + ":End of /NAMES list");
}

bool	Server::_isNickAvailable(string nick) const{
	vector<User>::const_iterator it;
	for (it = _users.begin() ; it != _users.end() ; ++it)
		if (it->getNick() == nick)
			return true;
	return false;
}

void Server::pass(User * user, string password) {
	if (user->isAuth())
		_sendTextToUser(NULL, user, "462 :You may not reregister");
	else if (password.empty())
		_sendTextToUser(NULL, user, "461 PASS :Not enough parameters");
	else {
		user->setPasswd(password);
		user->tryAuth();
	}
}

void Server::nick(User * user, string nickname) {
	if (nickname.empty())
		_sendTextToUser(NULL, user, "431 :No nickname given");
	else if (!_isNickAvailable(nickname))
		_sendTextToUser(NULL, user, "443 " + nickname + " :Nickname is already in use");
	else if (!_isValidNickname(nickname))
		_sendTextToUser(NULL, user, "432 " + nickname + " :Erroneus nickname");
	else {
		user->setNick(nickname);
		user->tryAuth();
	}
}

void Server::user(User * user, string userName, string hostName, string serverName, string realName) {
	(void)serverName;
	if (user->isAuth())
		_sendTextToUser(NULL, user, "462 :You may not reregister");
	else if (userName.empty() || hostName.empty() || serverName.empty() || realName.empty())
		_sendTextToUser(NULL, user, "461 PASS :Not enough parameters");
	else {
		user->setUserName(userName);
		user->setHostName(hostName);
		user->setRealName(realName);
		user->tryAuth();
	}
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

void Server::join(User * user, vector<string> & requested_channels) {
	vector<Channel>::iterator chan;
	vector<string>::iterator it;
	string topic;

	if (requested_channels.empty())
		_sendTextToUser(NULL, user, "461 JOIN :Not enough parameters");
	for (it = requested_channels.begin() ; it != requested_channels.end() ; ++it) {
		for (chan = _channels.begin() ; chan != _channels.end() ; ++chan)
			if (chan->getName() == *it)
				break;
		if (chan != _channels.end()) {
			if (!chan->addUser(user)) { // Cas où le channel est plein
				_sendTextToUser(NULL, user, "471  " + chan->getName() + " :Cannot join channel (+l)");
				return ;
			}
			topic = chan->getTopic();
			if (topic.empty()) // Cas où le topic est vide
				_sendTextToUser(NULL, user, "331 " + chan->getName() + " :No topic is set");
			else // Cas où il y a un topic
				_sendTextToUser(NULL, user, "332 " + chan->getName() + " :" + topic);
			_nameList(*chan, user);
		}
		else { // Cas où le channel n'existe pas encore
			_channels.push_back(Channel(*it));
			_channels.back().addUser(user);
			_channels.back().makeUserOp(user);
			_nameList(_channels.back(), user);
		}
	}
}

void Server::part(User * user, vector<string> & channels) {
	vector<string>::iterator it;
	vector<Channel>::iterator chan;

	if (channels.empty()) {
		_sendTextToUser(NULL, user, "461 PART :Not enough parameters");
		return ;
	}
	for (it = channels.begin() ; it != channels.end() ; ++it) {
		for (chan = _channels.begin() ; chan != _channels.end() && chan->getName() != *it ; ++chan)
			// On parcourt les channels jusqu'à trouver le bon ou avoir atteint la fin de la liste
			;
		if (chan->getName() == *it) { // Cas où on a trouvé le channel
			if (!chan->deleteUserFromChannel(user)) { // On essaie de supprimer le user de le channel
				_sendTextToUser(NULL, user, "442 " + *it + " :You're not on that channel");
				return ;
			}
		}
		else { // Cas où on n'a pas trouvé le channel
			_sendTextToUser(NULL, user, "403  " + *it + " :No such channel");
			return ;
		}
	}
}

string Server::mode(User * user, string requested_channel, vector<string> & operands) {
}

void Server::topic(User * user, string channel, string topic) {
	if (channel.empty()) {
		_sendTextToUser(NULL, user, "461 TOPIC :Not enough parameters");
		return ;
	}
	vector<Channel>::iterator it;
	for (it = _channels.begin() ; it != _channels.end() ; ++it) {
		if (it->getName() == channel)
			break ;
	}
	if (it == _channels.end() || !it->isUserInChannel(user)) {
		_sendTextToUser(NULL, user, "442 " + channel + " :You're not on that channel");
		return;
	}
	if (topic.empty()) {
		if (it->getTopic().empty())
			_sendTextToUser(NULL, user, "331 " + channel + " :No topic is set");
		else
			_sendTextToUser(NULL, user, "332 " + channel + " :" + it->getTopic()); 
	}
	else if (it->setTopic(user, topic))
		_sendTextToUser(NULL, user, "332 " + channel + " :" + it->getTopic());
	else
		_sendTextToUser(NULL, user, "482 " + channel + " :You're not channel operator");
}

void Server::_listChannel(User const * user, Channel const & channel) {
	if (!channel.isSecret() || channel.isUserInChannel(user)) {
		string topic = "";
		string chanName = "priv";
		if (!channel.isPrivate() || channel.isUserInChannel(user)) {
			chanName = channel.getName();
			topic = channel.getTopic();
		}
		_sendTextToUser(NULL, user, "322 " + chanName + ":" + topic); // RPL_LIST
	}
}

void Server::list(User * user, vector<string> & channels) {
	vector<string>::const_iterator it;
	vector<Channel>::const_iterator chan;

	_sendTextToUser(NULL, user, "321 Channel :Users Name"); // RPL_LISTSTART
	if (channels.empty()) // Cas où la commande n'a pas d'argument
		for (chan = _channels.begin() ; chan != _channels.end() ; ++chan)
			_listChannel(user, *chan);
	else // Cas où la commande a une liste d'arguments
		for (it = channels.begin() ; it != channels.end() ; ++it)
			for (chan = _channels.begin() ; chan != _channels.end() ; ++chan)
				if (chan->getName() == *it)
					_listChannel(user, *chan);
	_sendTextToUser(NULL, user, "323 :End of /LIST"); // RPL_LISTEND
}

void Server::kick(User * user, string channel, string kickee, string comment) {
	vector<Channel>::iterator chan;

	if (channel.empty() || kickee.empty()) {
		_sendTextToUser(NULL, user, "461 KICK :Not enough parameters");
		return ;
	}
	// On cherche le channel dans la liste
	for (chan = _channels.begin() ; chan != _channels.end() ; ++chan)
		if (chan->getName() != channel) // On a trouvé le bon channel
			break ;
	// Cas où le channel n'est pas trouvé
	if (chan == _channels.end()) {
		_sendTextToUser(NULL, user, "403 " + channel + " :No such channel");
		return ;
	}
	if (!chan->isUserOp(user))
		_sendTextToUser(NULL, user, "482 " + channel + " :You're not channel operator");
	else if (!chan->deleteUserFromChannel(kickee))
		_sendTextToUser(NULL, user, "441 " + kickee + " " + channel + " :They aren't on that channel");
	else { // Cas où la commande a réussi
		vector<User>::const_iterator targetUser;
		string kickMsg = "KICK " + channel + " " + kickee + (comment.empty()?"":" :" + comment);

		for (targetUser = _users.begin() ; targetUser != _users.end() ; ++targetUser)
			if (targetUser->getNick() == kickee)
				break ;
		_sendTextToUser(user, &*targetUser, kickMsg);
		_sendTextToChan(user, *chan, _composePrefix(user) + kickMsg);
	}
}

void Server::privmsg(User * user, vector<string> & recipients, string msg) {
	if (recipients.empty()) {
		_sendTextToUser(NULL, user, "411 :No recipient given (<PRIVMSG>)");
		return ;
	}
	if (msg.empty()) {
		_sendTextToUser(NULL, user, "412 :No text to send");
		return ;
	}
	vector<string>::iterator it;
	for (it = recipients.begin() ; it != recipients.end() ; ++it) {
		if ((*it)[0] == '#' || (*it)[0] == '&') // Case where the recipient is a channel
			_sendPrivmsgToChan(user, *it, msg);
		else // Case where the recipient is a user
			_sendPrivmsgToUser(user, *it, msg);
	}
}

void Server::notice(User * user, string recipient, string msg) {
	vector<User>::iterator it;
	vector<Channel>::iterator chan;

	if (recipient[0] == '#' || recipient[0] == '&') { // Case where the recipient is a channel
		for (chan = _channels.begin() ; chan != _channels.end() ; ++chan)
			if (chan->getName() == recipient)
				break;
		_sendTextToChan(user, *chan, _composePrefix(user) + "NOTICE " + recipient + " :" + msg);
	}
	else
		for (it = _users.begin() ; it != _users.end() ; ++it) {
			if (it->getNick() == recipient) {
				_sendTextToUser(user, &*it, "NOTICE " + recipient + " :" + msg);
				break;
			}
		}
}
