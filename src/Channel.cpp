#include "Channel.hpp"
#include <cstdlib>


Channel::Channel(std::string name, std::string key, Client* creator, Server *server): _name(name), _key(key), _topic(""), _server(server), _l(0), _i(false), _t(true), _topic_time(0), _creation_time(0)
{
	_operators.push_back(creator);
	_clients.push_back(creator);
	_mode_channels["+i"] = new AddInvite(this);
	_mode_channels["-i"] = new RemoveInvite(this);
	_mode_channels["+t"] = new AddTopic(this);
	_mode_channels["-t"] = new RemoveTopic(this);
	_mode_channels["+k"] = new AddKey(this);
	_mode_channels["-k"] = new RemoveKey(this);
	_mode_channels["+o"] = new AddOPerator(this);
	_mode_channels["-o"] = new RemoveOperator(this);
	_mode_channels["+l"] = new AddLimit(this);
	_mode_channels["-l"] = new RemoveLimit(this);
}

Channel::~Channel()	{
	_operators.clear();
	_clients.clear();
	delete _mode_channels["+i"];
	delete _mode_channels["-i"];
	delete _mode_channels["+t"];
	delete _mode_channels["-t"];
	delete _mode_channels["+k"];
	delete _mode_channels["-k"];
	delete _mode_channels["+o"];
	delete _mode_channels["-o"];
	delete _mode_channels["+l"];
	delete _mode_channels["-l"];
}

/* SETTERS */
void	Channel::setName(std::string name)	{ _name = name; }
void	Channel::setKey(std::string key)	{ _key = key; }
void	Channel::setTopic(std::string topic){ _topic = topic; }
void	Channel::setLimit(size_t limit)		{ _l = limit; }
void	Channel::setInviteMode(bool invite)	{ _i = invite; }
void	Channel::setTopicMode(bool topic)	{ _t = topic; }

/* BASIC GETTERS */
std::string				Channel::getName() const		{ return _name; }
std::string				Channel::getKey() const			{ return _key; }
std::string				Channel::getTopic() const		{ return _topic; }
std::vector<Client*>	Channel::getClients() const		{ return _clients; }
size_t					Channel::getLimit() const		{ return _l; }
bool					Channel::getInviteMode() const	{ return _i; }
bool					Channel::getTopicMode() const	{ return _t; }
std::vector<Client*>	Channel::getOperators() const	{ return _operators; }
Client					*Channel::getTopicSetter() const { return _topic_setter; }
time_t					Channel::getTopicTime() const	{ return _topic_time; }
time_t					Channel::getCreationTime() const { return _creation_time; }

/* ADVANCED GETTERS */
const std::string	Channel::getNamesList()
{
	std::string clients;
	for (std::vector<Client*>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
		Client *client = *it;
		clients += ((isOperator(client)) ? "@" : "") + (*it)->getNickname() + " ";
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

ModeChannel*	Channel::getModeChannel(std::string flag){
	ModeChannel *mdChan = NULL;
	ModeChannel_iterator it = _mode_channels.find(flag);
	if (it != _mode_channels.end())
		mdChan = it->second;
	return mdChan;
}


/* METHODS */
void					Channel::addClient(Client* client)
{
    if (std::find(_clients.begin(), _clients.end(), client) == _clients.end())
	{
        _clients.push_back(client);
	}
	// send to all except the new client
	broadcast(":" + client->getNickname() + "@" + client->getHostname() + " JOIN " + _name, client);
}

void	Channel::removeClient(Client* client)
{
    std::vector<Client*>::iterator it = std::find(_clients.begin(), _clients.end(), client);
    if (it != _clients.end()) {
        _clients.erase(it);
	}
	if (_clients.empty())
		return _server->removeChannel(this);
	if (isOperator(client))
		removeOperator(client);
	broadcast(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " PART " + _name + " :Goodbye", client);
	if (_operators.empty() && !(_clients.empty())) {
		addOperator(_clients.front());
	}
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
	client->send_response(RPL_NAMREPLY, client, "= " + getName() + " :" + getNamesList());
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

void Channel::execute_mode_channel(Client* client, std::vector<std::string> args)
{
	(void)client;

	if (args.size() == 3 && args[2].size() == 2)
	{
		ModeChannel *modeChannel = this->getModeChannel(args[2]);
		if (modeChannel == NULL)
			return client->send_response(ERR_UMODEUNKNOWNFLAG, client, " :Unknown MODE flag");
		else if (args[2][1] == 'o' || args[2] == "+k" || args[2] == "+l")
			return client->send_response(ERR_NEEDMOREPARAMS, client, " :Not enough parameters");
		modeChannel->execute("", 0);
		std::string response;
		response = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " MODE " + _name + " " + args[2];
		this->broadcast(response, NULL);
		return;
	}
	char	sign = args[2][0];
	int		index = 3;
	int		nb_args = args.size() - 2;
	for (size_t i = 1; i < args[2].size(); i++)
	{
		if (!(args[2][i] == 'i' || args[2][i] == 't' || args[2][i] == 'l' || args[2][i] == 'k'
			|| args[2][i] == 'o' || args[2][i] == '-' || args[2][i] == '+'))
			return client->send_response(ERR_UMODEUNKNOWNFLAG, client, " :Unknown MODE flag");
	}
	for (size_t i = 1; i < args[2].size(); i++)
	{
		std::string	flag;
		std::string	str = "";
		size_t		nb = 0;
		if (args[2][i] == '-' || args[2][i] == '+')
		{
			sign = args[2][i];
			continue;
		}
		flag.push_back(sign);
		flag.push_back(args[2][i]);
		ModeChannel *modeChannel = this->getModeChannel(flag);
		if (modeChannel == NULL)
			return client->send_response(ERR_UMODEUNKNOWNFLAG, client, " :Unknown MODE flag");
		if (flag[1] == 'o' || flag == "+k" || flag == "+l")
		{
			if (index >= nb_args + 2)
			{
				return client->send_response(ERR_NEEDMOREPARAMS, client, " :Not enough parameters");
			}
			if (flag[1] == 'o' || flag == "+k")
				str = args[index];
			else
				nb = static_cast<size_t>(std::atol(args[index].c_str()));
			index++;
		}
		modeChannel->execute(str, nb);
	}
		std::string response;
		response = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " MODE " + _name;
		for (size_t i = 2; i < args.size(); i++)
		{
			response += " " + args[i];
		}
		this->broadcast(response, NULL);
}

void Channel::setTopicSetter(Client* client) {
	_topic_setter = client;
}

void Channel::setTopicTime(time_t time) {
	_topic_time = time;
}

void Channel::setCreationTime(time_t time) {
	_creation_time = time;
}
