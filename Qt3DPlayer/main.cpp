#include "Qt3DPlayer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication player(argc, argv);
	player.setOrganizationName(QString("Senon 3D Metrology"));
	player.setOrganizationDomain(QString("https://www.linkedin.com/in/SenSprouting/"));
	player.setApplicationName(QString("Welcome to Senon 3D World!"));

	Qt3DPlayer widget;
	widget.resize(QSize(800, 500));
	widget.show();

	return player.exec();
}
