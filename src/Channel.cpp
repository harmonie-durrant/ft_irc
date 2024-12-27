#include "Channel.hpp"

Channel::Channel(std::string name, std::string key, Client* creator): _name(name), _key(key), _topic(""), _l(0), _i(false), _t(false)
{
	_operators.push_back(creator);
	_clients.push_back(creator);
}

Channel::~Channel()	{}

void	Channel::setName(std::string name)	{ _name = name; }
void	Channel::setKey(std::string key)	{ _key = key; }
void	Channel::setTopic(std::string topic){ _topic = topic; }
void	Channel::setLimit(size_t limit)		{ _l = limit; }

std::string	Channel::getName() const	{ return _name; }
std::string	Channel::getKey() const		{ return _key; }
std::string	Channel::getTopic() const	{ return _topic; }

Client*	Channel::getClient(std::string client_nickname)
{
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) 
	{
		if ((*it)->getNickname() == client_nickname)
			return *it;
	}
	return NULL;
}
size_t	Channel::getLimit() const			{ return _l; }
bool	Channel::getInviteMode() const		{ return _i; }
bool	Channel::getTopicMode() const		{ return _t; }

void	Channel::addClient(Client* client)
{
    if (std::find(_clients.begin(), _clients.end(), client) == _clients.end())
	{
        _clients.push_back(client);
	}
	// send to all except the new client
	std::string message = ":" + client->getNickname() + " JOIN " + _name;
	broadcast(message, client);
}

void	Channel::removeClient(Client* client)
{
    std::vector<Client*>::iterator it = std::find(_clients.begin(), _clients.end(), client);
    if (it != _clients.end())
	{
        _clients.erase(it);
	}
	// send to all except the leaving client
	std::string message = ":" + client->getNickname() + " PART " + _name;
	broadcast(message, client);
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
