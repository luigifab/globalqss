/**
 * Created M/25/11/2025
 * Updated D/25/01/2026
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

// GQSS_RELOAD GQSS_DEBUG GQSS_SET GQSS_THEME GQSS_READY
void GlobalQSS::polish(QApplication *app) {

	if (qEnvironmentVariableIsSet("GQSS_RELOAD")) {
		qunsetenv("GQSS_RELOAD");
		this->gqss_applied = false;
		if (qEnvironmentVariableIsSet("GQSS_DEBUG"))
			qDebug() << "GQSS: reload";
	}
	else if (this->gqss_applied) {
		// yolo
		return;
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
		themeName = cmd1.readAllStandardOutput().trimmed();
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
		themeName = cmd2.readAllStandardOutput().trimmed();
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
		QString css = "", version = QString::number(QT_VERSION_MAJOR);

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
						if (qEnvironmentVariableIsSet("GQSS_DEBUG"))
							qDebug() << "GQSS:  file" << filePath;

						if (f.open(QFile::ReadOnly)) {
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
					if (qEnvironmentVariableIsSet("GQSS_DEBUG"))
						qDebug() << "GQSS:  file" << filePath;

					if (f.open(QFile::ReadOnly)) {
						css += QString::fromUtf8(f.readAll()).replace("url(\"", "url(\"" + d.absolutePath() + "/") + "\n";
						f.close();
						break;
					}
				}
			}

			css = css.trimmed();
		}

		// apply QSS
		if (found) {

			this->gqss_applied = true;

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