/*
 * notificationPlugin.cpp
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

#include "notification/notificationPlugin.h"
#include "notification/notification.h"

NotificationPlugin::NotificationPlugin(const Coraline::Plugin::Context & ctx) :
Coraline::Plugin::Base(ctx)
{

	topWin = GTK_WINDOW(ctx.topWindow);
	Notification::getInstance()->setup(ctx);
}

NotificationPlugin::~NotificationPlugin() {

}
void NotificationPlugin::registerAllMethods() {
	PLUGINREGMETH(alert);
	PLUGINREGMETH(confirm);
	PLUGINREGMETH(prompt);
	PLUGINREGMETH(beep);
	PLUGINREGMETH(fileSelection);
}
/*
Coraline::Plugin::ClientCodeFileName NotificationPlugin::clientSideSupportFile() {
	return "init.js";
}
*/

void NotificationPlugin::startUp() {
	this->executeResourceJS("init.js");
}

bool NotificationPlugin::alert(
		const Coraline::Plugin::StandardCallbackIDs& callbacks,
		const Coraline::Plugin::ArgsList& args) {

	PLUGIN_CHECKARGSCOUNT("alert", 3, args);
	PLUGIN_CHECKARGTYPE(0, is_string, args);
	PLUGIN_CHECKARGTYPE(1, is_string, args);
	PLUGIN_CHECKARGTYPE(2, is_string, args);



	queueAction([callbacks, args, this](){

		Notification::getInstance()->alert(topWin, args[0], args[1], args[2]);
		reportSuccess(callbacks);

	});
	return true;
}


static ButtonLabelsList  jsonButtonsToLabelsList(json labelsArg) {

	ButtonLabelsList buttonLabels;
	if (labelsArg.size()) {
		for (uint8_t i=0; i< labelsArg.size(); i++) {
			std::string ho = labelsArg[i];
			buttonLabels.push_back(ho);
		}
	} else {

		buttonLabels.push_back("OK");
		buttonLabels.push_back("Cancel");
	}

	return buttonLabels;
}
bool NotificationPlugin::confirm(
		const Coraline::Plugin::StandardCallbackIDs& callbacks,
		const Coraline::Plugin::ArgsList& args) {

	PLUGIN_CHECKARGSCOUNT("confirm", 3, args);
	PLUGIN_CHECKARGTYPE(0, is_string, args);
	PLUGIN_CHECKARGTYPE(1, is_string, args);
	PLUGIN_CHECKARGTYPE(2, is_array, args);

	ButtonLabelsList buttonLabels = jsonButtonsToLabelsList(args[2]);

	queueAction([callbacks, args, buttonLabels, this](){

		int resp = Notification::getInstance()->confirm(topWin, args[0], args[1], buttonLabels);
		json val = resp;
		reportSuccess(callbacks, val);

	});
	return true;
}

bool NotificationPlugin::prompt(
		const Coraline::Plugin::StandardCallbackIDs& callbacks,
		const Coraline::Plugin::ArgsList& args) {

	PLUGIN_CHECKARGSCOUNT("confirm", 3, args);
	PLUGIN_CHECKARGTYPE(0, is_string, args);
	PLUGIN_CHECKARGTYPE(1, is_string, args);
	PLUGIN_CHECKARGTYPE(2, is_array, args);

	std::string defTxt;
	PLUGIN_OPTARG(3, is_string, args, defTxt);

	ButtonLabelsList buttonLabels = jsonButtonsToLabelsList(args[2]);

	queueAction([callbacks, args, buttonLabels, defTxt, this](){

		PromptReturn resp = Notification::getInstance()->prompt(topWin, args[0], args[1], buttonLabels, defTxt);
		json val;
		val["buttonIndex"] = resp.buttonIndex;
		val["input1"] = resp.input;
		reportSuccess(callbacks, val);

	});
	return true;

}

bool NotificationPlugin::beep(
		const Coraline::Plugin::StandardCallbackIDs& callbacks,
		const Coraline::Plugin::ArgsList& args) {

	reportError(callbacks);
	return false;
}

bool NotificationPlugin::fileSelection(const Coraline::Plugin::StandardCallbackIDs & callbacks,
						const Coraline::Plugin::ArgsList & args) {
	return false;
}

