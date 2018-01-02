/*
 * notification.h
 *
 *  Created on: Dec 25, 2017
 *           Copyright (C) 2017 Pat Deegan, https://psychogenic.com
 *
 *  This file is part of the Coraline Dialog plugin [https://coraline.psychogenic.com/]
 *
 *  Coraline is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Coraline is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Coraline.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef INCLUDE_NOTIFICATION_H_
#define INCLUDE_NOTIFICATION_H_

#include "notificationExtIncludes.h"

typedef std::string NotifString;
typedef std::vector<NotifString> ButtonLabelsList;

typedef struct {
	int buttonIndex;
	std::string input;
} PromptReturn;

class Notification {
public:
	static Notification * getInstance();

	void setup(const Coraline::Plugin::Context & ctx);


	void alert(GtkWindow * parentWindow, const NotifString & message,
			const NotifString & title="Alert",
			const NotifString & buttonLabel="OK");
	int confirm(GtkWindow * parentWindow, const NotifString & message,
			const NotifString & title,
			const ButtonLabelsList & buttonLabels);
	PromptReturn prompt(GtkWindow * parentWindow, const NotifString & message,
			const NotifString & title,
			const ButtonLabelsList & buttonLabels,
			const NotifString & defaultText="");


private:
	Notification();
	const char * iconFile;


};

#endif /* INCLUDE_NOTIFICATION_H_ */
