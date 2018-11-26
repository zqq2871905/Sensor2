#include "sensor.h"
#include <QtWidgets/QApplication>
//#include"ethernetopenclass.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Sensor w;
	w.OnInitial();

	w.show();
	return a.exec();
}
