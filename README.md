# ft_irc

<img src="https://github.com/mcombeau/mcombeau/blob/main/42_badges/ft_irce.png?raw=true" alt="ft_irc 42 project badge"/>

[![ft_irc CI](https://github.com/harmonie-durrant/ft_irc/actions/workflows/cpp.yml/badge.svg?branch=main)](https://github.com/harmonie-durrant/ft_irc/actions/workflows/cpp.yml)

## Comment lancer le serverur IRC ?

1. Clone le projet et naviguer dans le dossier crée

```bash
	git clone {url} {nom du dossier}
	cd {nom du dossier}
```

2. Construire le projet avec make puis lancer le serveur avec le port et mot de passe de ton choix.

```bash
	make
	./ircserv {port} {mot de passe]
```

## Comment se connecter avec le client IRC de référence ?

1. Installer le client IRC de référence irssi

```bash
	sudo apt-get install irssi
```

2. Lancer irssi et se connecter au serveur

```bash
	irssi
	/connect localhost {port} {mot de passe}
```

3. Rejoindre un channel

```bash
	/join {channel}
```

4. Envoyer un message

```bash
	/msg {channel} {message}
```

5. Quitter le channel

```bash
	/part {channel}
```

6. Quitter le serveur

```bash
	/quit
```

7. Quitter irssi

```bash
	/exit
```

8. Pour plus d'informations sur les commandes irssi, voir la [documentation](https://irssi.org/documentation/startup/)

## Auteurs

- [renflo974](https://github.com/renflo974)
- [fguillet42](https://github.com/fguillet42)
- [harmonie-durrant](https://www.github.com/harmonie-durrant)
