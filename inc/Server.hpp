/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mservage <mservage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 18:07:17 by mservage          #+#    #+#             */
/*   Updated: 2022/08/11 14:47:43 by mservage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <sstream>
#include <iostream>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <fcntl.h>
#include "Channel.hpp"
#include "User.hpp"

/* server contient:
	la liste des channel
	la liste des users
	la liste des fds des users (a mettre en struct pollfd pour poll)
	*/

using namespace std;

class Server
{
private:
	string			_server_name;
	string			_password;
	int				_port;
	sockaddr_in		_address;
	int				_addr_size;

	vector<Channel>	_channels;
	vector<User>	_users;
	vector<pollfd>	_fds;

	string			_composePrefix(User const * sender) const;
	string			_composeRplMessage(string errCode, User const * user) const;
	void			_sendPrivmsgToChan(User const * sender, string channel, string text) const;
	void			_sendPrivmsgToUser(User const * sender, string recipient, string text) const;
	void			_sendTextToChan(User const * sender, Channel const & chan, string text) const;
	void			_sendTextToUser(User const * sender, User const * recipient, string text) const;
	void			_nameList(Channel const & chan, User const * recipient) const;
	bool			_isNickAvailable(string nick) const;
	bool			_isLetter(char c) const;
	bool			_isSpecialCharacter(char c) const;
	bool			_isDigit(char c) const;
	bool			_isValidNickname(string nick) const;
	void			_listChannel(User const * user, Channel const & chan) const;
	void			_userMode(User * user, User * targetUser, vector<string> & operands);
	void			_channelMode(User * user, Channel * channel, vector<string> & operands);
	void			_displayChannelMode(User * user, Channel * channel);
	void			_displayWelcomeMessage(User * user);
	bool			_command_exists(string line);
	User *			_getUser(string nick);
	const User *	_getUser(string nick) const;
	void			_changeNicksInChan(string oldNick, string newNick);
	void			_botMsg(User * user, string msg) const;
	// Server commands
	void			pass(User * user, string password);
	void			nick(User * user, string nickname);
	void			user(User * user, string username, string hostname, string servername, string realname);
	void			quit(User * user, string msg);
	void			join(User * user, vector<string> & requested_channels);
	void			part(User * user, vector<string> & channels, string partMessage);
	void			mode(User * user, string requested_channel_or_user, vector<string> & operands);
	void			topic(User * user, string channel, string topic);
	void			list(User * user, vector<string> & channels) const;
	void			kick(User * user, string channel, string kickee, string comment);
	void			privmsg(User * user, vector<string> & recipients, string msg) const;
	void			notice(User * user, vector<string> &recipients, string msg) const;
	void			unknownCommand(User * user, string commandName) const;
	void			notLoggedIn(User * user) const;
	void			pong(User * user, string message) const;

public:
	Server();
	Server(Server const &src);
	Server(string server_name, int port, char *password);
	~Server();

	void			_send_txt(pollfd poll_fd, string text) const;
	Server			&operator=(Server const &rhs);
	int				getPort(void) const;
	string			getServerName(void) const;
	string			getPassword(void) const;
	vector<pollfd>	getFds();
	void			init_listen();
	void			wait_for_event();
	int				msg_parse(string buf, int index);
	string			getNextWord(string line, int *i) const;
	int				is_command(string command);
	vector<string>	getNextVector(string line, int *i);
	vector<std::string>	getNextVectorcomma(string line, int *i);
};
