/*
* Copyright Daniel Jones 2018
*
* This file is part of horriblesubs-rss.
*
* horriblesubs-rss is free software: you can redistribute it and/or modifiy
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Foobar is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QDomDocument>
#include <QCheckBox>
#include <QProcess>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

private slots:
    void on_get_button_clicked();

private:
		Ui::MainWindow *ui;
		bool getfile(QString url, QString dest);
		QNetworkAccessManager http;
		QEventLoop loop;
		bool parserss(QString rssfile);
		void addtolist(QString title, QString date, QString link);
		void openclient(QString magnet);

};

#endif // MAINWINDOW_H
