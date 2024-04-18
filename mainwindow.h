#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include "memory"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT
	QImage reference_image, transformed_image;

  public:
	explicit MainWindow(QWidget *parent = nullptr);

	~MainWindow() override;

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
//	Ui::MainWindow *ui;
//	QSlider *rot_slider;
//	QSlider *trans_x_slider;
//	QSlider *trans_y_slider;
//	QSlider *scale_slider;
	std::unique_ptr<Ui::MainWindow> ui;
	std::unique_ptr<QSlider> rot_slider, trans_x_slider, trans_y_slider, scale_slider;
	double last_y = 0;
	double last_x = 0;
	double last_alpha = 0;
	double last_scale = 100;

	void clearSliders();

	void setSliders();
};

#endif // MAINWINDOW_H
