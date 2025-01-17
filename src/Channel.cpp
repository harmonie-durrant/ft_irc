#include "Channel.hpp"

Channel::Channel(std::string name, std::string key, Client* creator, Server *server): _name(name), _key(key), _topic(""), _server(server), _l(0), _i(false), _t(false)
{
	_operators.push_back(creator);
	_clients.push_back(creator);
}

Channel::~Channel()	{}

/* SETTERS */
void	Channel::setName(std::string name)	{ _name = name; }
void	Channel::setKey(std::string key)	{ _key = key; }
void	Channel::setTopic(std::string topic){ _topic = topic; }
void	Channel::setLimit(size_t limit)		{ _l = limit; }

/* BASIC GETTERS */
std::string				Channel::getName() const		{ return _name; }
std::string				Channel::getKey() const			{ return _key; }
std::string				Channel::getTopic() const		{ return _topic; }
std::vector<Client*>	Channel::getClients() const		{ return _clients; }
size_t					Channel::getLimit() const		{ return _l; }
bool					Channel::getInviteMode() const	{ return _i; }
bool					Channel::getTopicMode() const	{ return _t; }

/* ADVANCED GETTERS */
std::string	Channel::getNamesList() const
{
	std::string clients;
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		clients += (*it)->getNickname() + " ";
	}
	return clients;
}

Client*	Channel::getClient(std::string client_nickname)
{
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if ((*it)->getNickname() == client_nickname)
			return *it;
	}
	return NULL;
}

/* METHODS */
void					Channel::addClient(Client* client)
{
    if (std::find(_clients.begin(), _clients.end(), client) == _clients.end())
	{
        _clients.push_back(client);
	}
	// send to all except the new client
	broadcast(":" + client->getNickname() + " JOIN " + _name, client);
}

void	Channel::removeClient(Client* client)
{
    std::vector<Client*>::iterator it = std::find(_clients.begin(), _clients.end(), client);
    if (it != _clients.end())
	{
        _clients.erase(it);
	}
	if (_clients.size() == 0)
		return _server->removeChannel(this);
	// send to all except the leaving client
	broadcast(":" + client->getNickname() + " PART " + _name, client);
}

void    Channel::addOperator(Client* client)
{
    if (std::find(_operators.begin(), _operators.end(), client) == _operators.end())
        _operators.push_back(client);
}

void    Channel::removeOperator(Client* client)
{
    std::vector<Client*>::iterator it = std::find(_operators.begin(), _operators.end(), client);
    if (it != _operators.end())
        _operators.erase(it);
}

bool Channel::isOperator(Client* client)
{
    return std::find(_operators.begin(), _operators.end(), client) != _operators.end();
}

bool Channel::isClient(Client* client)
{
    return std::find(_clients.begin(), _clients.end(), client) != _clients.end();
}

bool Channel::isInvited(Client* client)
{
	return std::find(_guests_list.begin(), _guests_list.end(), client) != _guests_list.end();
}

void Channel::invite(Client *client)
{
	_guests_list.push_back(client);
}

void Channel::uninvite(Client *client)
{
	std::vector<Client*>::iterator it = std::find(_guests_list.begin(), _guests_list.end(), client);
	if (it != _guests_list.end())
		_guests_list.erase(it);
}

void Channel::sendJoinSelf(Client* client)
{
	client->addChannel(this->getName());
	client->send_response(RPL_TOPIC, client, getName() + " :" + getTopic());
	// client->send_response(RPL_TOPICTIME, client, getName() + " :" + get_topic_time());
	client->send_response(RPL_NAMREPLY, client, getName() + " :" + getNamesList());
	client->send_response(RPL_ENDOFNAMES, client, getName() + " :End of /NAMES list");
}

void	Channel::broadcast(const std::string& message, Client* exclude)
{
    std::vector<Client*>::iterator it = _clients.begin();
    while (it != _clients.end())
    {
        if (exclude != NULL && *it == exclude)
        {
            ++it;
            continue;
        }
        (*it)->send_response(-1, *it, message);
        ++it;
    }
}

void Channel::kick(Client* kicker, Client* target, const std::string reason = "")
{
    removeClient(target);
    removeOperator(target);

    std::string message = ":" + kicker->getNickname() + " KICK " + _name + " " + target->getNickname() + " :" + reason;
    broadcast(message, NULL);

    target->send_response(-1, target, ":You have been kicked from " + _name + " by " + kicker->getNickname() + " (" + reason + ")");
}
