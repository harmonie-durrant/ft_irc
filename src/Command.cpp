#include "Command.hpp"

Command::Command(Server* server, bool auth) : _server(server), _auth(auth) {}

Command::~Command() {}

bool Command::auth_required() const {
    return _auth;
}