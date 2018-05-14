#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Qt3DPlayer.h"

class Qt3DPlayer : public QMainWindow
{
	Q_OBJECT

public:
	Qt3DPlayer(QWidget *parent = Q_NULLPTR);

private:
	Ui::Qt3DPlayerClass ui;
};
