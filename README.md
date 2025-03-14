# ft_irc

<img src="https://github.com/mcombeau/mcombeau/blob/main/42_badges/ft_irce.png?raw=true" alt="ft_irc 42 badge"/>

[![ft_irc CI](https://github.com/harmonie-durrant/ft_irc/actions/workflows/cpp.yml/badge.svg?branch=main)](https://github.com/harmonie-durrant/ft_irc/actions/workflows/cpp.yml)

A 42 project, the goal, to create your own IRC server in C++, fully compatible with an official client.

## How to launch the IRC server ?

1. Clone using git and navigate to the created folder

```bash
	git clone {url} {folder name}
	cd {folder name}
```

2. Use make to compile the project and then launch the server with the port and password of your choice.

```bash
	make
	./ircserv {port} {mot de passe}
```

## How to connect with the reference IRC client ?

1. Install the reference IRC client irssi

```bash
	sudo apt-get install irssi
```

2. Launch irssi and connect to the server

```bash
	irssi
	/connect localhost {port} {mot de passe}
```

3. Join a channel

```bash
	/join {channel}
```

4. Send a message

```bash
	/msg {channel} {message}
```

5. Leave the channel

```bash
	/part {channel}
```

6. Quit the program

```bash
	/quit
```
ou
```bash
	/exit
```

8. For more information on irssi commands, see the [documentation](https://irssi.org/documentation/startup/).

## Authors

- [renflo974](https://github.com/renflo974)
- [fguillet42](https://github.com/fguillet42)
- [harmonie-durrant](https://www.github.com/harmonie-durrant)
