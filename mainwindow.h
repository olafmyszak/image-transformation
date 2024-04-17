#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QSlider"
#include "QLayoutItem"
#include "QLayout"
#include "vector"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT
	QImage *reference_image, *transformed_image;

  public:
	MainWindow(QWidget *parent = nullptr);

	~MainWindow();

  public slots:

	void openImage(bool);

	void changeRotation(int);

	void openRotate(bool);

	void openTranslate(bool);

	void changeTranslationX(int);

	void changeTranslationY(int);

	void openScale(bool);

	void changeScale(int);

  private:
	Ui::MainWindow *ui;
	QSlider *rot_slider;
	QSlider *trans_x_slider;
	QSlider *trans_y_slider;
	QSlider *scale_slider;
	double last_y = 0;
	double last_x = 0;
	double last_alpha = 0;
	double last_scale = 100;

	void clearLayout(QLayout *layout);

	void setSliders();
};

#endif // MAINWINDOW_H
