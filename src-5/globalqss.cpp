/**
 * Created M/25/11/2025
 * Updated J/30/04/2026
 *
 * Copyright 2025-2026 | Fabrice Creuzot (luigifab) <code~luigifab~fr>
 * https://github.com/luigifab/globalqss
 * https://www.luigifab.fr/gtkqt/globalqss
 *
 * This program is free software, you can redistribute it or modify
 * it under the terms of the GNU General Public License (GPL) as published
 * by the free software foundation, either version 2 of the license, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but without any warranty, without even the implied warranty of
 * merchantability or fitness for a particular purpose. See the
 * GNU General Public License (GPL) for more details.
 */

#include "globalqss.h"

// can be defined from command line: GQSS_DEBUG GQSS_THEME
//      can be defined from program: GQSS_THEME GQSS_RELOAD
//      can be checked from program: GQSS_SET=yes (plugin loaded) / GQSS_READY=yes (plugin loaded and theme applied) / GQSS_THEME (theme name)
//                                   GQSS_SIGNAL=yes (on desktop theme change)
//
//   engine is loaded without theme: GQSS_SET="yes" + GQSS_THEME=""
// engine is loaded with None theme: GQSS_SET="yes" + GQSS_READY="yes" + GQSS_THEME="None" (empty style sheet)
//      engine is loaded with theme: GQSS_SET="yes" + GQSS_READY="yes" + GQSS_THEME="abc"  (theme files and qt.qss)

GlobalQSS::~GlobalQSS() {

	qunsetenv("GQSS_SET");
	qunsetenv("GQSS_READY");

	if (gqss_monitor)
		QDBusConnection::sessionBus().disconnect(
			"ca.desrt.dconf", "/ca/desrt/dconf/Writer/user", "ca.desrt.dconf.Writer", "Notify",
			this, SLOT(onNotify(QString)));
}

void GlobalQSS::onNotify(const QString &path) {

	qDebug() << "GQSS: onNotify" << path;

	if (qApp && ((path == "/org/gnome/desktop/interface/gtk-theme") || (path == "/org/mate/desktop/interface/gtk-theme"))) {

		if (qEnvironmentVariableIsSet("GQSS_DEBUG"))
			qDebug() << "GQSS: themeChanged";

		qunsetenv("GQSS_THEME");
		qputenv("GQSS_SIGNAL", "yes");

		gqss_applied = false;
		polish(qApp); // apply theme

		qunsetenv("GQSS_SIGNAL");
	}
}

void GlobalQSS::polish(QApplication *app) {

	if (qEnvironmentVariableIsSet("GQSS_RELOAD")) {
		qunsetenv("GQSS_RELOAD");
		gqss_applied = false;
		if (qEnvironmentVariableIsSet("GQSS_DEBUG"))
			qDebug() << "GQSS: reload";
	}
	else if (gqss_applied) {
		return; // yolo
	}

	// @see https://github.com/loot/loot/issues/1896
	// start monitoring of desktop theme change with dbus (disabled when application is started with GQSS_THEME=xyz)
	// QDBusConnectionPrivate() got message (signal): QDBusMessage(type=Signal, service=":1.10", path="/ca/desrt/dconf/Writer/user", interface="ca.desrt.dconf.Writer", member="Notify", signature="sass", contents=("/org/mate/desktop/interface/gtk-theme", {""}, ":1.10:user:xyz") )
	if (!gqss_monitor && !qEnvironmentVariableIsSet("GQSS_SET") && !qEnvironmentVariableIsSet("GQSS_THEME")) {

		gqss_monitor = QDBusConnection::sessionBus().connect(
			"ca.desrt.dconf", "/ca/desrt/dconf/Writer/user", "ca.desrt.dconf.Writer", "Notify",
			this, SLOT(onNotify(QString)));

		if (qEnvironmentVariableIsSet("GQSS_DEBUG")) {
			if (gqss_monitor)
				qDebug() << "GQSS: monitor started";
			else
				qDebug() << "GQSS: monitor error" << QDBusConnection::sessionBus().lastError().message();
		}
	}

	qputenv("GQSS_SET", "yes");

	// GQSS_THEME
	QString themeName = qEnvironmentVariable("GQSS_THEME").trimmed();

	// MATE
	// if found, set GQSS_THEME
	if (themeName.isEmpty()) {

		QProcess cmd1;
		cmd1.start("gsettings", QStringList() << "get" << "org.mate.interface" << "gtk-theme");
		cmd1.waitForFinished(200);
		themeName = QString::fromUtf8(cmd1.readAllStandardOutput()).trimmed();
		cmd1.close();

		if (!themeName.isEmpty()) {
			if (themeName.startsWith("'") && themeName.endsWith("'"))
				themeName = themeName.mid(1, themeName.length() - 2);
			if (!themeName.isEmpty())
				qputenv("GQSS_THEME", themeName.toUtf8());
		}
	}

	// GNOME
	// if found, set GQSS_THEME
	if (themeName.isEmpty()) {

		QProcess cmd2;
		cmd2.start("gsettings", QStringList() << "get" << "org.gnome.desktop.interface" << "gtk-theme");
		cmd2.waitForFinished(200);
		themeName = QString::fromUtf8(cmd2.readAllStandardOutput()).trimmed();
		cmd2.close();

		if (!themeName.isEmpty()) {
			if (themeName.startsWith("'") && themeName.endsWith("'"))
				themeName = themeName.mid(1, themeName.length() - 2);
			if (!themeName.isEmpty())
				qputenv("GQSS_THEME", themeName.toUtf8());
		}
	}

	// load and apply theme
	// if found and applied, set GQSS_READY
	// else, unset GQSS_READY
	if (themeName.isEmpty()) {
		qunsetenv("GQSS_READY");
	}
	else {
		bool found = (themeName == "None");
		QString css, version = QString::number(QT_VERSION_MAJOR);

		if (qEnvironmentVariableIsSet("GQSS_DEBUG"))
			qDebug() << "GQSS: theme" << themeName;

		// do nothing (None theme) or load theme (theme files and qt.qss)
		if (!found) {

			// load from theme files
			for (const QString &path : {
				QDir::homePath() + "/.themes/" + themeName + "/qt" + version,
				"/usr/share/themes/" + themeName + "/qt" + version,
				"/usr/local/share/themes/" + themeName + "/qt" + version
			}) {

				QDir d(path);
				if (qEnvironmentVariableIsSet("GQSS_DEBUG"))
					qDebug() << "GQSS:   dir" << path;

				if (d.exists()) {

					d.setNameFilters(QStringList() << "*.qss");
					d.setFilter(QDir::Files);

					QStringList files = d.entryList();
					files.sort(Qt::CaseInsensitive);
					for (const QString &fileName : files) {

						QString filePath = d.absoluteFilePath(fileName);
						QFile f(filePath);

						if (f.open(QFile::ReadOnly)) {

							if (qEnvironmentVariableIsSet("GQSS_DEBUG"))
								qDebug() << "GQSS:  file" << filePath;

							css += QString::fromUtf8(f.readAll()).trimmed().replace("url(\"", "url(\"" + d.absolutePath() + "/") + "\n";
							f.close();
						}
					}

					found = true;
					break;
				}
			}

			// load from qt.qss
			for (const QString &path : {
				QDir::homePath() + "/.config/qt" + version
			}) {

				QDir d(path);
				if (qEnvironmentVariableIsSet("GQSS_DEBUG"))
					qDebug() << "GQSS:   dir" << path;

				if (d.exists()) {

					QString filePath = d.absoluteFilePath("qt.qss");
					QFile f(filePath);

					if (f.open(QFile::ReadOnly)) {

						if (qEnvironmentVariableIsSet("GQSS_DEBUG"))
							qDebug() << "GQSS:  file" << filePath;

						css += QString::fromUtf8(f.readAll()).trimmed().replace("url(\"", "url(\"" + d.absolutePath() + "/") + "\n";
						f.close();
						break;
					}
				}
			}

			css = css.trimmed();
		}

		// apply QSS
		if (found) {

			gqss_applied = true;

			qputenv("GQSS_READY", "yes");
			if (qEnvironmentVariableIsSet("GQSS_DEBUG"))
				qDebug() << "GQSS: setStyleSheet";

			// @todo
			//for (QWidget *top : app->topLevelWidgets())
			//	top->setProperty("textDirection", (app->layoutDirection() == Qt::RightToLeft) ? "rtl" : "ltr");

			//app->setStyleSheet(css); // vlc = crash
			if (QCoreApplication::applicationName().toLower().contains("vlc"))
				QMetaObject::invokeMethod(app, "setStyleSheet", Qt::QueuedConnection, Q_ARG(QString, css));
			else
				QMetaObject::invokeMethod(app, "setStyleSheet", Qt::DirectConnection, Q_ARG(QString, css));
		}
		else {
			qunsetenv("GQSS_READY");
		}
	}
}