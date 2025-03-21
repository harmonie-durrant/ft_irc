#include "Command.hpp"
#include "Server.hpp"

Topic::Topic(Server* server, bool auth) : Command(server, auth) {}

Topic::~Topic() {}

void Topic::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response(ERR_NEEDMOREPARAMS, client, args[0] + " :Not enough parameters");
		return;
	}
	std::string channel_name = args[1];
	std::string new_topic = "";
	if (args.size() > 2 && args[2][0] == ':')
	{
		new_topic = args[2].substr(1);
		for (std::size_t i = 3; i < args.size(); i++)
			new_topic += " " + args[i];
	}
	Channel *channel = _server->getChannel(channel_name);
	if (channel == NULL)
		return client->send_response(ERR_NOSUCHCHANNEL, client, channel_name + " :No such channel");
	if (channel->isClient(client) == false)
		return client->send_response(ERR_NOTONCHANNEL, client, channel_name + " :You're not on that channel");
	std::string topic = channel->getTopic();
	if (new_topic.empty() && args.size() == 2) {
		if (topic.empty())
			return client->send_response(RPL_NOTOPIC, client, channel_name + " :No topic is set");
		client->send_response(RPL_TOPIC, client, channel_name + " :" + topic);
		std::stringstream ss;
		ss << channel->getTopicSetter()->getNickname() << " " << channel->getTopicTime();
		return client->send_response(RPL_TOPICWHOTIME, client, channel_name + " " + ss.str());
	}
	if (channel->isOperator(client) == false && channel->getTopicMode() == true)
		return client->send_response(ERR_CHANOPRIVSNEEDED, client, channel_name + " :You're not a channel operator");
	channel->setTopic(new_topic);
	channel->setTopicSetter(client);
	channel->setTopicTime(Server::getTimestamp());
	return channel->broadcast(":" + client->getNickname() + " TOPIC " + channel_name + " :" + new_topic, NULL);
}
