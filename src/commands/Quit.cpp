#include "Command.hpp"
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"

Quit::Quit(Server* server, bool auth) : Command(server, auth) {}

Quit::~Quit() {}

void Quit::execute(Client* client, std::vector<std::string> args) {
	(void)args;
	// lors qu un client QUIT : envoyer message sur SERVER
	// est effectuee par client_disconnect()?
	// envoyer un message sur tous les channels auquels le clients est connecte
	// exple msg sur channel : froque [~froque@169.155.250.35] has quit [Client Quit]
	// si il est le dernier membre du channel il faut le fermer
	_server->client_disconnect(client->getFd());
}
