/*
 * notification.cpp
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
#include "notification/notification.h"

#define NOTIF_PADDING_DEFAULT	20
#define NOTIF_WIDTH_DEFAULT		350
#define NOTIF_HEIGH_DEFAULT		200

void ho2(GtkWindow* pwin) {
	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;

	dialog = gtk_file_chooser_dialog_new ("Open File",
	                                      pwin,
	                                      action,
	                                      "Cancel",
	                                      GTK_RESPONSE_CANCEL,
	                                      "Open",
	                                      GTK_RESPONSE_ACCEPT,
	                                      NULL);
	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT)
	  {
	    char *filename;
	    GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
	    filename = gtk_file_chooser_get_filename (chooser);

	    g_free (filename);
	  }

	gtk_widget_destroy (dialog);
}

Notification* Notification::getInstance() {
	static Notification* notif_singleton = NULL;
	if (notif_singleton) {
		return notif_singleton;
	}

	notif_singleton = new Notification();
	return notif_singleton;
}

void Notification::setup(const Coraline::Plugin::Context & ctx) {
	iconFile= ctx.application.icon;

}

void addLabel(const NotifString & message, GtkWidget * dialog_widget, const char* iconFile) {


	GtkWidget * content_area =  gtk_dialog_get_content_area (GTK_DIALOG(dialog_widget));

	GtkWidget* iconImg = NULL;

    /* --- Create a label with the message --- */
	GtkWidget *label = gtk_label_new (message.c_str());

	if (iconFile) {
		iconImg = gtk_image_new_from_file(iconFile);
	}

	if (iconImg) {

		GtkWidget * hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
		gtk_box_pack_start(GTK_BOX(hbox), iconImg, FALSE, FALSE, 10);
		gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 10);

	    /* --- Add label to the dialog --- */
	    gtk_box_pack_start (GTK_BOX (content_area),
	                        hbox, TRUE, TRUE, 0);

	} else {

    /* --- Add label to the dialog --- */
    gtk_box_pack_start (GTK_BOX (content_area),
                        label, TRUE, TRUE, 0);
	}


}

void Notification::alert(GtkWindow* parentWindow, const NotifString& message,
		const NotifString& title, const NotifString& buttonLabel) {
	GtkDialogFlags flags = GTK_DIALOG_MODAL;
	GtkWidget * alert_dialog =
	gtk_dialog_new_with_buttons (title.c_str(),
	                             parentWindow,
								 flags,
								 buttonLabel.c_str(),
								 GTK_RESPONSE_ACCEPT,
								 NULL);


    gtk_window_set_modal( GTK_WINDOW( alert_dialog ), TRUE );
    gtk_window_set_transient_for( GTK_WINDOW( alert_dialog ),
                                  GTK_WINDOW( parentWindow ) );


    gtk_window_set_default_size (GTK_WINDOW(alert_dialog),
    							NOTIF_WIDTH_DEFAULT, NOTIF_HEIGH_DEFAULT);
    // gtk_window_set_type_hint()

    addLabel(message, alert_dialog, iconFile);

    /* --- Show the dialog --- */
    gtk_widget_show_all(alert_dialog);
    gtk_dialog_run(GTK_DIALOG(alert_dialog));
    /* --- Close it. --- */
    gtk_widget_destroy (alert_dialog);

}

static GtkWidget * setupBasicDialog(GtkWindow * parentWindow,
		const NotifString & title,
		const ButtonLabelsList & buttonLabels){


	GtkWidget * dialog_widget = gtk_dialog_new();
	GtkWindow * dialog_window = GTK_WINDOW(dialog_widget);
	gtk_window_set_title(dialog_window, title.c_str());


    gtk_window_set_modal(dialog_window, TRUE );
    gtk_window_set_transient_for(dialog_window, parentWindow);


    gtk_window_set_default_size (dialog_window,
    							NOTIF_WIDTH_DEFAULT, NOTIF_HEIGH_DEFAULT);
    unsigned int buttonIdx = 0;
    while (buttonIdx < buttonLabels.size())
    {



	    gtk_dialog_add_button (GTK_DIALOG(dialog_window),
	                           buttonLabels[buttonIdx].c_str(),
							   (buttonIdx + 1));

	    buttonIdx++;
    }





    return dialog_widget;


}


int Notification::confirm(GtkWindow * parentWindow, const NotifString & message,
			const NotifString & title,
			const ButtonLabelsList & buttonLabels) {

			GtkWidget * dialog_widget = setupBasicDialog(parentWindow, title, buttonLabels);


			addLabel(message, dialog_widget, iconFile);

		    /* --- Show the dialog --- */
		    gtk_widget_show_all(dialog_widget);
		    gint resp = gtk_dialog_run(GTK_DIALOG(dialog_widget));
		    /* --- Close it. --- */
		    gtk_widget_destroy (dialog_widget);

		    if (resp < 1) {
		    	return 0;
		    }

		    return resp;

}


PromptReturn Notification::prompt(GtkWindow * parentWindow, const NotifString & message,
			const NotifString & title,
			const ButtonLabelsList & buttonLabels, const NotifString & defaultText) {
	PromptReturn retVal;



	GtkWidget * dialog_widget = setupBasicDialog(parentWindow, title, buttonLabels);

    /* --- Create a label with the message --- */


	addLabel(message, dialog_widget, iconFile);


	GtkWidget * content_area =  gtk_dialog_get_content_area (GTK_DIALOG(dialog_widget));
	GtkWidget * entryInput = gtk_entry_new();

	if (defaultText.size()) {
		gtk_entry_buffer_set_text(gtk_entry_get_buffer(GTK_ENTRY(entryInput)), defaultText.c_str(), defaultText.size());

	}

	gtk_box_pack_end(GTK_BOX (content_area), entryInput, TRUE, TRUE, NOTIF_PADDING_DEFAULT);


    /* --- Show the dialog --- */
    gtk_widget_show_all(dialog_widget);


    retVal.buttonIndex =  gtk_dialog_run(GTK_DIALOG(dialog_widget));
    retVal.input = gtk_entry_get_text(GTK_ENTRY(entryInput));

    /* --- Close it. --- */
    gtk_widget_destroy (dialog_widget);

    if (retVal.buttonIndex < 1) {
    	retVal.buttonIndex = 0;
    }

    return retVal;


}

Notification::Notification() : iconFile(NULL){
}
