#include <iostream>

static int start_server(int port, const char *password)
{
	std::cout << "Starting server on port " << port << " with password '" << password << "'" << std::endl;
	/* Start server here and listen for end events and connections etc.. */
	return 0;
}

int main(int ac, char const *av[])
{
	int res = 0;

	if (ac != 3) {
		std::cerr << "Usage: " << av[0] << " <port> <password>" << std::endl;
		return 1;
	}
	res = start_server(3000, "password");
	std::cout << "Server ended with code: " << res << std::endl;
	return res;
}
