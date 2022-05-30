/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 18:07:17 by mservage          #+#    #+#             */
/*   Updated: 2022/05/30 18:44:40 by lgaudet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <poll.h>
#include <sys/socket.h>
#include "Channel.hpp"

/* server contient:
	la liste des channel
	la liste des users
	la liste des fds des users (a mettre en struct pollfd pour poll)
	*/

using namespace std;

class Server
{
private:
	string		_server_name;
	int				_port;

	vector<Channel>	_channels;
	vector<User>		_users;
	vector<pollfd>		_fds;

	// Server commands
	string pass(User * user, string password);
	string nick(User * user, string nickname);
	string user(User * user, string username, string hostname, string servername, string realname);
	// oper command ?
	void quit(User * user, string msg);
	string join(User * user, vector<string> & requested_channels, vector<string> & passwords);
	string part(User * user, vector<string> & channels);
	string mode(User * user, string requested_channel, vector<string> & operands);
	string topic(User * user, string channel, string topic);
	string list(User * user, vector<string> & channels);
	string invite(User * user, string nick, string channel);
	string kick(User * user, string channel, string kickee, string comment);
	string privmsg(User * user, vector<string> & recipients, string msg);
	void notice(User * user, string recipient, string msg);

public:
	Server();
	Server(Server const &src);
	Server(string server_name, int port);
	~Server();

	Server	&operator=(Server const &rhs);
	int	getPort(void) const;
	string	getServerName(void) const;
	vector<pollfd> getFds();
	void	addNewUser(string name, string password);
};
