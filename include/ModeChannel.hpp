/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeChannel.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fguillet <fguillet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 13:05:44 by froque            #+#    #+#             */
/*   Updated: 2025/01/17 10:09:12 by froque           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Header protection
#pragma once

// Includes
#include <iostream>
#include <string>
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"
#include "Server.hpp"
#include "Client.hpp"

class ModeChannel
{
	protected:
		class Channel*	_channel;
		ModeChannel();
		ModeChannel(const ModeChannel &copy);

	public:
		/* CONSTRUCT DECONSTRUCT */
		explicit ModeChannel(class Channel* channel);
		virtual ~ModeChannel();
		/* GETTERS */
		/* METHODS */
		virtual void	execute(std::string pwd, size_t limit) = 0;
};


/* MODE CLASSES */
class AddInvite : public ModeChannel
{
public:
	AddInvite(class Channel *channel);
	~AddInvite();

	void	execute(std::string pwd, size_t limit);
};

class RemoveInvite : public ModeChannel
{
public:
	RemoveInvite(class Channel *channel);
	~RemoveInvite();

	void	execute(std::string pwd, size_t limit);
};
class AddTopic : public ModeChannel
{
public:
	AddTopic(class Channel *channel);
	~AddTopic();

	void	execute(std::string pwd, size_t limit);
};

class RemoveTopic : public ModeChannel
{
public:
	RemoveTopic(class Channel *channel);
	~RemoveTopic();

	void	execute(std::string pwd, size_t limit);
};

class AddKey : public ModeChannel
{
public:
	AddKey(class Channel *channel);
	~AddKey();

	void	execute(std::string pwd, size_t limit);
};

class RemoveKey : public ModeChannel
{
public:
	RemoveKey(class Channel *channel);
	~RemoveKey();

	void	execute(std::string pwd, size_t limit);
};

class AddOPerator : public ModeChannel
{
public:
	AddOPerator(class Channel *channel);
	~AddOPerator();

	void	execute(std::string pwd, size_t limit);
};

class RemoveOperator : public ModeChannel
{
public:
	RemoveOperator(class Channel *channel);
	~RemoveOperator();

	void	execute(std::string pwd, size_t limit);
};

class AddLimit : public ModeChannel
{
public:
	AddLimit(class Channel *channel);
	~AddLimit();

	void	execute(std::string pwd, size_t limit);
};

class RemoveLimit : public ModeChannel
{
public:
	RemoveLimit(class Channel *channel);
	~RemoveLimit();

	void	execute(std::string pwd, size_t limit);
};
