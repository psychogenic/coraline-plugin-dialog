/*
 * notificationPlugin.h
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

#ifndef INCLUDE_NOTIFICATION_NOTIFICATIONPLUGIN_H_
#define INCLUDE_NOTIFICATION_NOTIFICATIONPLUGIN_H_
#include "notification/notificationExtIncludes.h"

#define MYPLUGIN_VERSION_MAJOR			1
#define MYPLUGIN_VERSION_MINOR 			0
#define MYPLUGIN_VERSION_PATCH 			3


class NotificationPlugin : public Coraline::Plugin::Base {
public:
	NotificationPlugin(const Coraline::Plugin::Context & ctx);
	virtual ~NotificationPlugin();

	virtual void startUp();

	Coraline::Version version() { return Coraline::Version();}

    virtual const std::string merges() { return "navigator.notification";}
    virtual const Coraline::PluginName fullName() { return "cordova-plugin-dialogs"; }
    virtual const Coraline::PluginName shortName() { return "Notification";}
	// virtual Coraline::Plugin::ClientCodeFileName clientSideSupportFile();

protected:
	virtual void registerAllMethods();

private:
	GtkWindow * topWin;

	bool alert(const Coraline::Plugin::StandardCallbackIDs & callbacks,
							const Coraline::Plugin::ArgsList & args);
	bool confirm(const Coraline::Plugin::StandardCallbackIDs & callbacks,
							const Coraline::Plugin::ArgsList & args);
	bool prompt(const Coraline::Plugin::StandardCallbackIDs & callbacks,
							const Coraline::Plugin::ArgsList & args);
	bool beep(const Coraline::Plugin::StandardCallbackIDs & callbacks,
							const Coraline::Plugin::ArgsList & args);

	bool fileSelection(const Coraline::Plugin::StandardCallbackIDs & callbacks,
							const Coraline::Plugin::ArgsList & args);


};

#endif /* INCLUDE_NOTIFICATION_NOTIFICATIONPLUGIN_H_ */
