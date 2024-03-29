/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mservage <mservage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 22:01:07 by lgaudet-          #+#    #+#             */
/*   Updated: 2022/08/11 14:47:24 by mservage         ###   ########.fr       */
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
	res.append("@" + sender->getHostName() + " ");
	return res;
}

string Server::_composeRplMessage(string errCode, User const * user) const {
	return errCode + " " + (user->getNick().empty()? "" : user->getNick() + " ");
}

void Server::_sendPrivmsgToChan(User const * sender, string channel, string text) const{
	vector<Channel>::const_iterator chan;
	vector<User*>::const_iterator it;
	for (chan = _channels.begin() ; chan != _channels.end() ; ++chan)
		if (chan->getName() == channel)
			break;
	if (chan == _channels.end())
		_sendTextToUser(NULL, sender, _composeRplMessage("401", sender) + channel + " :No such nick/channel");
	else if (!chan->canUserMessageChannel(sender->getNick()))
		_sendTextToUser(NULL, sender, _composeRplMessage("404", sender) + channel + " :Cannot send to channel");
	else
		_sendTextToChan(sender, *chan, _composePrefix(sender) + "PRIVMSG " + channel + " :" + text);
}

void Server::_sendPrivmsgToUser(User const * sender, string recipient, string text) const{
	vector<User>::const_iterator it;
	for (it = _users.begin() ; it != _users.end() ; ++it)
		if (it->getNick() == recipient)
			break; // the correct User is found
	if (it == _users.end()) {
		_sendTextToUser(NULL, sender, _composeRplMessage("401", sender) + recipient + " :No such nick/channel");
		return ;
	}
	_sendTextToUser(sender, &*it, "PRIVMSG " + recipient + " :" + text);
}

void Server::_sendTextToChan(User const * sender, Channel const & chan, string text) const{
	vector<string>::const_iterator it;

	for (it = chan.membersBegin() ; it != chan.membersEnd() ; ++it)
		if (sender->getNick() != *it)
			_send_txt(_getUser(*it)->getPollFd(), text);
}

void Server::_sendTextToUser(User const * sender, User const * recipient, string text) const{
	_send_txt(recipient->getPollFd(), _composePrefix(sender) + text);
}

void	Server::_nameList(Channel const & chan, User const * recipient) const {
	vector<string>::const_iterator member;
	for (member = chan.membersBegin() ; member != chan.membersEnd() ; member++) {
		bool isOp = chan.isUserChanOp(*member);
		_sendTextToUser(NULL, recipient, _composeRplMessage("353", recipient) + chan.getName() + " :" + (isOp?"@":"+") + *member);
	}
	_sendTextToUser(NULL, recipient, _composeRplMessage("366", recipient) + chan.getName() + " :End of /NAMES list");
}

bool	Server::_isNickAvailable(string nick) const{
	vector<User>::const_iterator it;
	for (it = _users.begin() ; it != _users.end() ; ++it)
		if (it->getNick() == nick)
			return false;
	return true;
}

bool Server::_isLetter(char c) const {
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return true;
	return false;
}

bool Server::_isSpecialCharacter(char c) const {
	if ((c >= 0x5B && c <= 60) || (c >= 0x7B && c <= 0x7D))
		return true;
	return false;
}

bool Server::_isDigit(char c) const {
	if (c >= '0' && c <= '9')
		return true;
	return false;
}

bool Server::_isValidNickname(string nick) const {
	if (nick.size() > 9 || nick.empty())
		return false;
	string::const_iterator it = nick.begin();
	if (!(_isLetter(*it) || _isSpecialCharacter(*it)))
		return false;
	for (it = nick.begin() + 1 ; it != nick.end() ; ++it)
		if (!(_isLetter(*it) || _isDigit(*it) || _isSpecialCharacter(*it) || *it == '-'))
			return false;
	return true;
}

void	Server::_displayWelcomeMessage(User * user) {
	_sendTextToUser(NULL, user, _composeRplMessage("001", user) +
			"Welcome to the Internet Relay Network " + user->getNick() + "!" +
			user->getUserName() + "@" + user->getHostName());
	_sendTextToUser(NULL, user, _composeRplMessage("002", user) + "Your host is " +
			this->_server_name + ", running version 0.0.0.42");
	_sendTextToUser(NULL, user, _composeRplMessage("003", user) + "This server was created some time ago");
	_sendTextToUser(NULL, user, _composeRplMessage("004", user) + this->_server_name +
			" 0.0.0.42 r onpstl");
}

bool _isValidChannelName(string name) {
	if (name.size() < 2)
		return false;
	if (name[0] != '#')
		return false;
	return true;
}

User * Server::_getUser(string nick) {
	vector<User>::iterator it;

	for (it = _users.begin() ; it != _users.end() ; ++it)
		if (it->getNick() == nick)
			return &*it;
	return NULL;
}

const User * Server::_getUser(string nick) const {
	vector<User>::const_iterator it;

	for (it = _users.begin() ; it != _users.end() ; ++it)
		if (it->getNick() == nick)
			return &*it;
	return NULL;
}

void Server::pass(User * user, string password) {
	if (user->isAuth())
		_sendTextToUser(NULL, user, _composeRplMessage("462", user) + ":You may not reregister");
	else if (password.empty())
		_sendTextToUser(NULL, user, _composeRplMessage("461", user) + "PASS :Not enough parameters");
	else if (password != this->_password)
		_sendTextToUser(NULL, user, _composeRplMessage("464", user) + ":Password incorrect");
	else {
		user->setPasswd(password);
		if (user->tryAuth(this->_password))
			_displayWelcomeMessage(user);
	}
}

void Server::_changeNicksInChan(string oldNick, string newNick) {
	vector<Channel>::iterator chan;

	for (chan = _channels.begin() ; chan != _channels.end() ; ++chan)
		chan->changeNickOfUser(oldNick, newNick);
}

void Server::nick(User * user, string nickname) {
	vector<User>::const_iterator user_it;

	if (nickname.empty())
		_sendTextToUser(NULL, user, _composeRplMessage("431", user) + ":No nickname given");
	else if (!_isNickAvailable(nickname))
		_sendTextToUser(NULL, user, _composeRplMessage("433", user) + nickname + " :Nickname is already in use");
	else if (!_isValidNickname(nickname))
		_sendTextToUser(NULL, user, _composeRplMessage("432", user) + nickname + " :Erroneus nickname");
	else {

		for (user_it = _users.begin() ; user_it != _users.end() ; ++user_it)
			if (user->isAuth())
				_sendTextToUser(user, &*user_it, "NICK " + nickname);
		if (user->isAuth())
			_changeNicksInChan(user->getNick(), nickname);
		user->setNick(nickname);
		if (!user->isAuth() && user->tryAuth(this->_password))
			_displayWelcomeMessage(user);
	}
}

void Server::user(User * user, string userName, string hostName, string serverName, string realName) {
	(void)serverName;
	if (user->isAuth())
		_sendTextToUser(NULL, user, _composeRplMessage("462", user) + ":You may not reregister");
	else if (userName.empty() || hostName.empty() || serverName.empty() || realName.empty())
		_sendTextToUser(NULL, user, _composeRplMessage("461", user) + "PASS :Not enough parameters");
	else {
		if (hostName == "*")
			user->setHostName("localhost");
		else
			user->setHostName(hostName);
		user->setUserName(userName);
		user->setRealName(realName);
		if (user->tryAuth(this->_password))
			_displayWelcomeMessage(user);
	}
}

void Server::quit(User * user, string msg) {
	vector<Channel>::iterator chan;

	msg = msg.empty()? user->getNick() + " is quitting the server" : msg;
	for (chan = _channels.begin() ; chan != _channels.end() ; ++chan)
		if (chan->isUserInChannel(user->getNick())) {
			chan->deleteUserFromChannel(user->getNick());
			_sendTextToChan(user, *chan, _composePrefix(user) + "QUIT :" + msg);
			if (chan->membersBegin() == chan->membersEnd()) {// Le channel est vide
				_channels.erase(chan);
				chan = _channels.begin();
				chan--;
			}
		}
}

void Server::join(User * user, vector<string> & requested_channels) {
	vector<Channel>::iterator chan;
	vector<string>::const_iterator it;
	vector<string>::const_iterator user_it;
	string topic;

	if (requested_channels.empty())
		_sendTextToUser(NULL, user, _composeRplMessage("461", user) + "JOIN :Not enough parameters");
	for (it = requested_channels.begin() ; it != requested_channels.end() ; ++it) {
		for (chan = _channels.begin() ; chan != _channels.end() ; ++chan)
			if (chan->getName() == *it)
				break;
		if (!_isValidChannelName(*it)) {
			_sendTextToUser(NULL, user, _composeRplMessage("403", user) + *it + " :No such channel");
			return ;
		}
		_sendTextToUser(user, user, "JOIN " + *it);
		if (chan != _channels.end()) { // Cas où le channel existe
			// On vérifie si le user est déjà dans le channel
			for (user_it = chan->membersBegin() ; user_it != chan->membersEnd() ; ++user_it)
				if (*user_it == user->getNick())
					break ;
			if (user_it != chan->membersEnd()) {// Cas où le user est déjà dans le channel
				_sendTextToUser(NULL, user, _composeRplMessage("443", user) + chan->getName() + " :is already on channel");
				return ;
			}
			if (!chan->addUser(user->getNick())) { // Cas où le channel est plein
				_sendTextToUser(NULL, user, _composeRplMessage("471", user) + chan->getName() + " :Cannot join channel (+l)");
				return ;
			}
			_sendTextToChan(user, *chan, _composePrefix(user) + "JOIN " + chan->getName());
			topic = chan->getTopic();
			if (topic.empty()) // Cas où le topic est vide
				_sendTextToUser(NULL, user, _composeRplMessage("331", user) + chan->getName() + " :No topic is set");
			else // Cas où il y a un topic
				_sendTextToUser(NULL, user, _composeRplMessage("332", user) + chan->getName() + " :" + topic);
			_nameList(*chan, user);
		}
		else { // Cas où le channel n'existe pas encore
			_channels.push_back(Channel(*it));
			_channels.back().addUser(user->getNick());
			_channels.back().setUserChanOp(user->getNick(), true);
			_sendTextToUser(NULL, user, _composeRplMessage("331", user) + _channels.back().getName() + " :No topic is set");
			_nameList(_channels.back(), user);
		}
	}
}

void Server::part(User * user, vector<string> & channels, string partMessage) {
	vector<string>::iterator it;
	vector<Channel>::iterator chan;
	vector<User*>::const_iterator op;

	partMessage = partMessage.empty() ? "A user has left the channel" : partMessage;
	if (channels.empty()) {
		_sendTextToUser(NULL, user, _composeRplMessage("461", user) + "PART :Not enough parameters");
		return ;
	}
	for (it = channels.begin() ; it != channels.end() ; ++it) {
		std::cout << *it << std::endl;
		for (chan = _channels.begin() ; chan != _channels.end() && chan->getName() != *it ; ++chan)
			// On parcourt les channels jusqu'à trouver le bon ou avoir atteint la fin de la liste
			;
		if (chan != _channels.end()) { // Cas où on a trouvé le channel
			if (!chan->deleteUserFromChannel(user->getNick())) { // On essaie de supprimer le user de le channel
				_sendTextToUser(NULL, user, _composeRplMessage("442", user) + *it + " :You're not on that channel");
				return ;
			}
			_sendTextToUser(user, user, "PART " + chan->getName());
			_sendTextToChan(user, *chan, _composePrefix(user) + "PART " + chan->getName() + " :" + partMessage);
			// On vérifie qu'il reste au moins un opérateur de channel, et on supprime le channel s'il est vide
			if (chan->membersBegin() == chan->membersEnd()) { // Le channel est vide
				_channels.erase(chan);
				chan = _channels.begin();
				chan--;
			}
		}
		else { // Cas où on n'a pas trouvé le channel
			_sendTextToUser(NULL, user, _composeRplMessage("403", user) + *it + " :No such channel");
			return ;
		}
	}
}

void Server::_userMode(User * user, User * targetUser, vector<string> & operands) {
	(void)targetUser;
	if (operands.empty())
		_sendTextToUser(NULL, user, _composeRplMessage("221", user));
	else
		_sendTextToUser(NULL, user, _composeRplMessage("501", user) + "MODE :Unknown MODE flag");
}

void Server::_displayChannelMode(User * user, Channel * channel) {
	string res = _composeRplMessage("324", user) + channel->getName();
	string params;
	string mode = " +";

	if (!channel->isMessageFromOutsideAllowed())
		mode += "n";
	if (channel->isPrivate())
		mode += "p";
	if (channel->isSecret())
		mode += "s";
	if (channel->isTopicSettableOnlyByOp())
		mode += "t";
	if (channel->getMaxNbOfUser() != 0) {
		mode += "l";
		params += " " + to_string(channel->getMaxNbOfUser());
	}
	mode += params; // On ajoute les paramètres à la fin de la chaîne de mode
	if (mode == " +") // Si la chaîne de mode est vide, on n'affiche pas le '+'
		mode.clear();
	_sendTextToUser(NULL, user, res + mode); // On envoie la chaîne complétée à l'utilisateur
}

void Server::_channelMode(User * user, Channel * channel, vector<string> & operands) {
	if (operands.empty()) {
		_displayChannelMode(user, channel);
		return ;
	}

	bool add;

	string modeString = operands[0];
	vector<string>::const_iterator currOp = operands.begin() + 1;
	if (modeString[0] == '-')
		add = false;
	else if (modeString[0] == '+')
		add = true;
	else {
		_sendTextToUser(NULL, user, _composeRplMessage("472", user) + modeString[0] + " :is unknown mode char to me for " + channel->getName());
		return ;
	}
	for (size_t i = 1 ; i < modeString.size() ; ++i) {
		if (modeString[i] == 'o') {
			if (currOp->empty()) {
				_sendTextToUser(NULL, user, _composeRplMessage("461", user) + "MODE :Not enough parameters");
				return ;
			}
			User * targetUser = _getUser(*(currOp++));
			if (!targetUser) {
				_sendTextToUser(NULL, user, _composeRplMessage("441", user) + *(currOp - 1) + " " + channel->getName() + " :They aren't on that channel");
				return ; 
			}
			if ((user == targetUser && !add) || channel->isUserChanOp(user->getNick())) {
				channel->setUserChanOp(targetUser->getNick(), add);
				_sendTextToChan(user, *channel, _composePrefix(user) + "MODE " + channel->getName() + " " + (add?"+":"-") + "o " + targetUser->getNick());
				_sendTextToUser(user, user, "MODE " + channel->getName() + " " + (add?"+":"-") + "o " + targetUser->getNick());
			}
			else {
				_sendTextToUser(NULL, user, _composeRplMessage("482", user) + channel->getName() + " :You're not channel operator");
				return ;
			}
		}
		else if (!channel->isUserChanOp(user->getNick())) {
			_sendTextToUser(NULL, user, _composeRplMessage("482", user) + channel->getName() + " :You're not channel operator");
			return ;
		}
		else if (modeString[i] == 'n')
			channel->setMessageFromOutsideAllowed(user->getNick(), !add);
		else if (modeString[i] == 'p')
			channel->setPrivate(user->getNick(), add);
		else if (modeString[i] == 's')
			channel->setSecret(user->getNick(), add);
		else if (modeString[i] == 't')
			channel->setTopicSettableOnlyByOp(user->getNick(), add);
		else if (modeString[i] == 'l') {
			if (add) {
				if (currOp->empty()) {
					_sendTextToUser(NULL, user, _composeRplMessage("461", user) + "MODE :Not enough parameters");
					return ;
				}
				stringstream ss;
				int requestedNumberOfUsers;
				ss << *(currOp++);
				if (!ss.fail()) {
					ss >> requestedNumberOfUsers;
					if (requestedNumberOfUsers > 0)
						channel->setMaxNbOfUsers(user->getNick(), requestedNumberOfUsers);
				}
			}
			else
				channel->setMaxNbOfUsers(user->getNick(), 0);
		}
		else {
			_sendTextToUser(NULL, user, _composeRplMessage("472", user) + modeString[i] + " :is unknown mode char to me for " + channel->getName());
			return ;
		}
	}
	_displayChannelMode(user, channel);
}

void Server::mode(User * user, string requested_channel_or_user, vector<string> & operands) {
	vector<User>::iterator requestedUser;
	vector<Channel>::iterator requestedChannel;

	if (requested_channel_or_user.empty()) {
		_sendTextToUser(NULL, user, _composeRplMessage("461", user) + "MODE :Not enough parameters");
		return ;
	}
	for (requestedUser = _users.begin() ; requestedUser != _users.end() ; ++requestedUser)
		if (requestedUser->getNick() == requested_channel_or_user)
			break ; // The correct user has been found
	if (requestedUser != _users.end()) { // If the user has been found, try to apply requested changes
		_userMode(user, &*requestedUser, operands);
		return ;
	}
	for (requestedChannel = _channels.begin() ; requestedChannel != _channels.end() ; ++ requestedChannel)
		if (requestedChannel->getName() == requested_channel_or_user)
			break ; // The correct channel has been found
	if (requestedChannel != _channels.end()) {
		_channelMode(user, &*requestedChannel, operands);
		return ;
	}
	else {
		_sendTextToUser(NULL, user, _composeRplMessage("401", user) + requested_channel_or_user + ":No such nick/channel");
		return ;
	}
}

void Server::topic(User * user, string channel, string topic) {
	if (channel.empty()) {
		_sendTextToUser(NULL, user, _composeRplMessage("461", user) + "TOPIC :Not enough parameters");
		return ;
	}
	vector<Channel>::iterator it;
	for (it = _channels.begin() ; it != _channels.end() ; ++it) {
		if (it->getName() == channel)
			break ;
	}
	if (it == _channels.end() || !it->isUserInChannel(user->getNick())) {
		_sendTextToUser(NULL, user, _composeRplMessage("442", user) + channel + " :You're not on that channel");
		return;
	}
	if (topic.empty()) {
		if (it->getTopic().empty())
			_sendTextToUser(NULL, user, _composeRplMessage("331", user) + channel + " :No topic is set");
		else
			_sendTextToUser(NULL, user, _composeRplMessage("332", user) + channel + " :" + it->getTopic()); 
	}
	else if (it->setTopic(user->getNick(), topic)) {
		vector<string>::const_iterator chanUser;
		for (chanUser = it->membersBegin() ; chanUser != it->membersEnd() ; ++chanUser)
			_sendTextToUser(NULL, _getUser(*chanUser), _composeRplMessage("332", _getUser(*chanUser)) + channel + " :" + it->getTopic());
	}
	else
		_sendTextToUser(NULL, user, _composeRplMessage("482", user) + channel + " :You're not channel operator");
}

void Server::_listChannel(User const * user, Channel const & channel) const {
	if (!channel.isSecret() || channel.isUserInChannel(user->getNick())) {
		string topic = "";
		string chanName = "priv";
		if (!channel.isPrivate() || channel.isUserInChannel(user->getNick())) {
			chanName = channel.getName();
			topic = channel.getTopic();
		}
		_sendTextToUser(NULL, user, _composeRplMessage("322", user) + chanName + " :" + topic); // RPL_LIST
	}
}

void Server::list(User * user, vector<string> & channels) const {
	vector<string>::const_iterator it;
	vector<Channel>::const_iterator chan;

	_sendTextToUser(NULL, user, _composeRplMessage("321", user) + "Channel :Users Name"); // RPL_LISTSTART
	if (channels.empty()) // Cas où la commande n'a pas d'argument
		for (chan = _channels.begin() ; chan != _channels.end() ; ++chan)
			_listChannel(user, *chan);
	else // Cas où la commande a une liste d'arguments
		for (it = channels.begin() ; it != channels.end() ; ++it)
			for (chan = _channels.begin() ; chan != _channels.end() ; ++chan)
				if (chan->getName() == *it)
					_listChannel(user, *chan);
	_sendTextToUser(NULL, user, _composeRplMessage("323", user) + ":End of /LIST"); // RPL_LISTEND
}

void Server::kick(User * user, string channel, string kickee, string comment) {
	vector<Channel>::iterator chan;

	if (channel.empty() || kickee.empty()) {
		_sendTextToUser(NULL, user, _composeRplMessage("461", user) + "KICK :Not enough parameters");
		return ;
	}
	// On cherche le channel dans la liste
	for (chan = _channels.begin() ; chan != _channels.end() ; ++chan)
		if (chan->getName() == channel) // On a trouvé le bon channel
			break ;
	// Cas où le channel n'est pas trouvé
	if (chan == _channels.end()) {
		_sendTextToUser(NULL, user, _composeRplMessage("403", user) + channel + " :No such channel");
		return ;
	}
	if (!chan->isUserChanOp(user->getNick())) // On vérifie que l'utilisateur a le droit de faire l'opération
		_sendTextToUser(NULL, user, _composeRplMessage("482", user) + channel + " :You're not channel operator");
	else if (!chan->deleteUserFromChannel(kickee)) // On essaie de kick la cible
		_sendTextToUser(NULL, user, _composeRplMessage("441", user) + kickee + " " + channel + " :They aren't on that channel");
	else { // Cas où la commande a réussi
		vector<User>::const_iterator targetUser;
		string kickMsg = "KICK " + channel + " " + kickee + (comment.empty()?"":" :" + comment);

		for (targetUser = _users.begin() ; targetUser != _users.end() ; ++targetUser)
			if (targetUser->getNick() == kickee)
				break ;
		_sendTextToUser(user, &*targetUser, kickMsg);
		_sendTextToUser(user, user, kickMsg);
		_sendTextToChan(user, *chan, _composePrefix(user) + kickMsg);
	}
}

void Server::_botMsg(User * user, string msg) const {
	string joke[3] = {"Qu’est-ce qui est rouge et mauvais pour les dents ? Une brique !", "Casse-toi tu pues, et marche à l’ombre", "Qu’est-ce qui est petit et marron ? Un marron !"};

	srand(time(NULL));
	if (msg == "pardon")
		_send_txt(user->getPollFd(), ":jeanMichel!bot@localhost NOTICE " + user->getNick() + " :" + "Bon toutou.");
	else
		_send_txt(user->getPollFd(), ":jeanMichel!bot@localhost NOTICE " + user->getNick() + " :" + joke[rand() % 3]);
}

void Server::privmsg(User * user, vector<string> & recipients, string msg) const{
	if (recipients.empty()) {
		_sendTextToUser(NULL, user, _composeRplMessage("411", user) + ":No recipient given (<PRIVMSG>)");
		return ;
	}
	if (msg.empty()) {
		_sendTextToUser(NULL, user, _composeRplMessage("412", user) + ":No text to send");
		return ;
	}
	vector<string>::const_iterator it;
	for (it = recipients.begin() ; it != recipients.end() ; ++it) {
		if ((*it)[0] == '#' || (*it)[0] == '&') // Case where the recipient is a channel
			_sendPrivmsgToChan(user, *it, msg);
		else if (*it == "jeanMichel")
			_botMsg(user, msg);
		else // Case where the recipient is a user
			_sendPrivmsgToUser(user, *it, msg);
	}
}

void Server::notice(User * user, vector<string> & recipients, string msg) const {
	vector<User>::const_iterator user_it;
	vector<Channel>::const_iterator chan;
	vector<string>::const_iterator it;

	if (recipients.empty() || msg.empty())
		return ;
	for (it = recipients.begin() ; it != recipients.end() ; ++it) {
		if ((*it)[0] == '#' || (*it)[0] == '&') { // Case where the recipient is a channel
			for (chan = _channels.begin() ; chan != _channels.end() ; ++chan)
				if (chan->getName() == *it)
					break;
			_sendTextToChan(user, *chan, _composePrefix(user) + "NOTICE " + *it + " :" + msg);
		}
		else if (*it == "jeanMichel")
			_botMsg(user, msg);
		else
			for (user_it = _users.begin() ; user_it != _users.end() ; ++user_it) {
				if (user_it->getNick() == *it) {
					_sendTextToUser(user, &*user_it, "NOTICE " + *it + " :" + msg);
					break;
				}
			}
	}
}

void Server::unknownCommand(User * user, string commandName) const{
	_sendTextToUser(NULL, user, _composeRplMessage("421", user) + commandName + " :Unknown command");
}

void Server::notLoggedIn(User * user) const {
	_sendTextToUser(NULL, user, _composeRplMessage("451", user) + ":You have not registered");
}

void Server::pong(User * user, string message) const {
	_sendTextToUser(NULL, user, "PONG :" + message);
}
