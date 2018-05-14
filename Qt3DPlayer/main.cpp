#include "Qt3DPlayer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Qt3DPlayer w;
	w.show();
	return a.exec();
}
