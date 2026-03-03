/**
 * Created M/25/11/2025
 * Updated J/01/01/2026
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

#ifndef GLOBALQSS_H
#define GLOBALQSS_H

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QProxyStyle>
#include <QString>
#include <QStringList>
#include <QWidget>

class GlobalQSS : public QProxyStyle
{
	Q_OBJECT
	bool gqss_applied = false;

public:
	GlobalQSS() = default;
	void polish(QApplication *app) override;
};

#endif