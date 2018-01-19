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
