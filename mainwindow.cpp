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
 * horriblesubs-rss is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with horriblesubs-rss.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	//ui->table_view->horizontalHeader()->
	//setSectionResizeMode(QHeaderView::Stretch);	// enable column resizing
	QPalette palette;				// palette for table
	palette.setColor(QPalette::Highlight,
		ui->table_view->
		palette().color(QPalette::Base));	
	palette.setColor(QPalette::HighlightedText,
		ui->table_view->palette().color(
			QPalette::Text));
	ui->table_view->setPalette(palette);		// set table color palette
	QString rssfile = "rss.txt";			// rss file to read and store to
	if (getfile("http://horriblesubs.info/rss.php?res=720"
				,rssfile))		// download rss file
		ui->status_bar->showMessage(
				"RSS feed downloaded");	// download complete
	else
		ui->status_bar->
		showMessage("Cannot download RSS feed");// download fail
	parserss(rssfile);				// fail or complete, try and parse file
}

MainWindow::~MainWindow()
{
	delete ui;
}

bool MainWindow::getfile(QString url, QString dest)
{
	bool success;					// return bool
	if (QFile::exists(dest))
		QFile::remove(dest);			// remove rss file if already exists
	QNetworkReply *reply = http.get(QNetworkRequest(
				url));			// network reply
	QObject::connect(reply, SIGNAL(finished()),
			&loop, SLOT(quit()));		// connect finish signal to quit
	loop.exec();					// exec loop
	QFile file(dest);
	file.open(QIODevice::WriteOnly);		// open file for writing
	file.write(reply->readAll());			// write reply data to file
	delete reply;					// delete replt object
	if (QFile::exists(dest) && file.size() > 0)	// this is a terrible way to do this
		success = true;				// file downloaded successfully
	else
		success = false;			// download failed
	return success;
}

bool MainWindow::parserss(QString rssfile)
{
	QDomDocument doc;				// xml parser
	QFile file(rssfile);				// rss file
	if (!file.open(QIODevice::ReadOnly) ||
			!doc.setContent(&file))
		return false;				// file not opened
	QDomNodeList title =
		doc.elementsByTagName("item");		// item tag
	qDebug() << title.size();
	for (int i = 0; i < title.size(); i++)		// iterate over every item
	{
		QDomNode n = title.item(i);		// current title
		QDomElement title =
			n.firstChildElement("title");	// get title
		QDomElement link =
			n.firstChildElement("link");	// get link to current item
		QDomElement date =
			n.firstChildElement("pubDate");	// get pub date
		addtolist(title.text(),
			date.text(), link.text());	// add current item to list
	}
	return true;
}

void MainWindow::addtolist(QString title, QString date, QString link)
{
	int i = ui->table_view->rowCount();		// get row count
	ui->table_view->insertRow(i);			// insert new row for item
	ui->table_view->setItem
	(i, 1, new QTableWidgetItem("title"));		// add title item
	ui->table_view->setItem
	(i, 2, new QTableWidgetItem("date"));		// add date item
	ui->table_view->setItem
	(i, 3, new QTableWidgetItem("link"));		// add link item
	ui->table_view->setItem
	(i, 0, new QTableWidgetItem("grab"));		// add grab item

	ui->table_view->item(i,1)->setText(title);	// add title text
	ui->table_view->item(i,2)->setText(date);	// add date test
	ui->table_view->item(i,3)->setText(link);	// add link text
	QTableWidgetItem *checkbox =
			new QTableWidgetItem();		// create checkbox item
    	checkbox->setCheckState(Qt::Unchecked);		// set checkbox state
	ui->table_view->setItem(i, 0, checkbox);	// place checkbox inside row
}

void MainWindow::on_get_button_clicked()
{
	int x = ui->table_view->rowCount();		// get row count
	for (int i = 0; i < x; i++)			// iterate over each item in the table
	{
		if (ui->table_view->item(i, 0)->	// check if checkbox.. checked
			checkState() == 2)		// checked is Qt::CheckState(Checked) (2)
		{
			openclient(
			ui->table_view->item(i, 3)
			->text());			// call open client with the magnet url
		}
	}
}

void MainWindow::openclient(QString magnet)
{
	QProcess *process = new QProcess();		// create process
	QString program = "/usr/bin/transmission-qt";	// this is built for transmission
	QStringList arg;				// argument list
	arg.append(magnet);				// append magnet to arg
        process->start(program, arg);			// start process
}
