/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addTopic.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froque <froque@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 13:23:24 by froque            #+#    #+#             */
/*   Updated: 2025/01/17 13:23:25 by froque           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ModeChannel.hpp"
#include "Channel.hpp"
#include "numeric_error.hpp"
#include "numeric_rpl.hpp"

RemoveOperator::RemoveOperator(Channel* channel) : ModeChannel(channel) {}

RemoveOperator::~RemoveOperator() {}

void RemoveOperator::execute(std::string pwd, size_t limit){

	(void)pwd;
	(void)limit;
	_channel->removeOperator(_channel->getClient(pwd));
}
