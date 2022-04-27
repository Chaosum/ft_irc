/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mservage <mservage@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 18:07:17 by mservage          #+#    #+#             */
/*   Updated: 2022/04/25 18:07:46 by mservage         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include "User.hpp"
#include "Channel.hpp"

/* server contient:
	un tableau de class channel
	la liste des users
	*/

class Server
{
private:
	
	std::vector<Channel>	_channels;
	std::vector<User>		_users;
public:
	Server();
	Server(Server const &src);
	~Server();

	Server	&operator=(Server const &rhs);
}