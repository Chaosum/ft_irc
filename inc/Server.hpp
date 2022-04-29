/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matthieu <matthieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 18:07:17 by mservage          #+#    #+#             */
/*   Updated: 2022/04/29 13:57:38 by matthieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <poll.h>
#include <sys/socket.h>
#include "User.hpp"
#include "Channel.hpp"

/* server contient:
	la liste des channel
	la liste des users
	la liste des fds des users (a mettre en struct pollfd pour poll)
	*/

class Server
{
private:
	std::string		_server_name;
	int				_port;

	std::vector<Channel>	_channels;
	std::vector<User>		_users;
	std::vector<pollfd>		_fds;

public:
	Server();
	Server(Server const &src);
	Server(std::string server_name, int port);
	~Server();

	Server	&operator=(Server const &rhs);
	int	getPort(void) const;
	std::string	getServerName(void) const;
	void	addNewUser(std::string name, std::string password);
};
