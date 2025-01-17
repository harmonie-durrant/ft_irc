#include "Command.hpp"
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"

Mode::Mode(Server* server, bool auth) : Command(server, auth) {}

Mode::~Mode() {}

void	Mode::modeChannel(class Client* client, class Channel* channel, std::vector<std::string> args)
{

}

void Mode::execute(Client* client, std::vector<std::string> args) {
	if (args.size() < 2)
	{
		client->send_response(ERR_NEEDMOREPARAMS, client, args[0] + " :Not enough parameters");
		return;
	}
	std::string nick = client->getNickname();
	std::string servername = client->getServerName();
	if (args.size() == 2)
	{
		std::string channel_name = args[1];
		Channel* channel = _server->getChannel(channel_name);
		if (channel == NULL)
			return client->send_response(ERR_NOSUCHCHANNEL, client, channel_name + " :No such channel");
		std::string response;
		response = channel_name;
		if (channel->getInviteMode() || channel->getTopicMode() || channel->getKey().length() > 0)
			response += " +";
		if (channel->getInviteMode())
			response += "i";
		if (channel->getTopicMode())
			response += "t";
		if (channel->getKey().length() > 0)
			response += "k " + channel->getKey();
		client->send_response(RPL_CHANNELMODEIS, client, response); //! Send the channel modes
		// voir commentaire pied de page
		return;
	}
	else
	{
		std::string channel_name = args[1];
		Channel* channel = _server->getChannel(channel_name);
		if (channel == NULL)
			return client->send_response(ERR_NOSUCHCHANNEL, client, channel_name + " :No such channel");
		if (channel->isOperator(client) == false)
			return client->send_response(ERR_CHANOPRIVSNEEDED, client, channel_name + " :You're not channel operator");
		char sign = args[2][0];
		if (!(sign == '+' || sign == '-'))
			return client->send_response(ERR_UMODEUNKNOWNFLAG, client, " :Unknown MODE flag");
		//modeChannel(client, channel, args);
	}
	client->send_response(-1, client, ":" + servername + " MODE " + nick + " " + args[1]);
}


// avoir si on doit gerer
// :irc.example.com 329 YourNick #example 1693587253
// 329 : Timestamp de création du canal (1693587253 est un horodatage UNIX).
// a gerer /mode YourNick +i ?  car irssi envoie ca a la connection
