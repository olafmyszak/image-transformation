#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"

#include "image_processing.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
	ui(new Ui::MainWindow), rot_slider(nullptr), trans_x_slider(nullptr), trans_y_slider(nullptr), scale_slider(nullptr)
{
	ui->setupUi(this);

	connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openImage(bool)));
	connect(ui->actionRotate, SIGNAL(triggered(bool)), this, SLOT(openRotate(bool)));
	connect(ui->actionTranslate, SIGNAL(triggered(bool)), this, SLOT(openTranslate(bool)));
	connect(ui->actionScale, SIGNAL(triggered(bool)), this, SLOT(openScale(bool)));
}

MainWindow::~MainWindow() = default;

void MainWindow::openImage(bool)
{
	if (QString name = QFileDialog::getOpenFileName(); !name.isEmpty())
	{
		clearTransformationMatrix();

		reference_image = QImage(name);
		transformed_image = reference_image;

		ui->src->setPixmap(QPixmap::fromImage(reference_image));
		ui->dst->setPixmap(QPixmap::fromImage(transformed_image));

		last_x = last_y = last_alpha = 0;
		last_scale = 100;

		setSliders();
	}
}

void MainWindow::openRotate(bool)
{
	if (!reference_image.isNull())
	{
		clearSliders();

		rot_slider = std::make_unique<QSlider>(Qt::Horizontal);

		setSliders();

		ui->verticalLayout->addWidget(rot_slider.get());

		connect(rot_slider.get(), SIGNAL(valueChanged(int)), this, SLOT(changeRotation(int)));
	}
}

void MainWindow::changeRotation(int alpha)
{
	const double new_alpha = alpha / 1000.0;

	translate(transformed_image.width() / 2.0, transformed_image.height() / 2.0);
	rotate(new_alpha - last_alpha);

	last_alpha = new_alpha;
	translate(-transformed_image.width() / 2.0, -transformed_image.height() / 2.0);

	applyTransformations(reference_image, transformed_image);

	ui->dst->setPixmap(QPixmap::fromImage(transformed_image));
}

void MainWindow::openTranslate(bool)
{
	if (!reference_image.isNull())
	{
		clearSliders();

		trans_x_slider = std::make_unique<QSlider>(Qt::Horizontal);
		trans_y_slider = std::make_unique<QSlider>(Qt::Horizontal);

		setSliders();

		ui->verticalLayout->addWidget(trans_x_slider.get());
		ui->verticalLayout->addWidget(trans_y_slider.get());

		connect(trans_x_slider.get(), SIGNAL(valueChanged(int)), this, SLOT(changeTranslationX(int)));
		connect(trans_y_slider.get(), SIGNAL(valueChanged(int)), this, SLOT(changeTranslationY(int)));
	}
}

void MainWindow::changeTranslationX(int dx)
{
	const double new_x = dx / 10.0;

	rotate(-last_alpha);

	translate(new_x - last_x, 0);
	last_x = new_x;

	rotate(last_alpha);

	applyTransformations(reference_image, transformed_image);
	ui->dst->setPixmap(QPixmap::fromImage(transformed_image));
}

void MainWindow::changeTranslationY(int dy)
{
	const double new_y = dy / 10.0;

	rotate(-last_alpha);

	translate(0, new_y - last_y);

	last_y = new_y;

	rotate(last_alpha);

	applyTransformations(reference_image, transformed_image);

	ui->dst->setPixmap(QPixmap::fromImage(transformed_image));
}

void MainWindow::openScale(bool)
{
	if (!reference_image.isNull())
	{
		clearSliders();

		scale_slider = std::make_unique<QSlider>(Qt::Horizontal);

		setSliders();

		ui->verticalLayout->addWidget(scale_slider.get());

		connect(scale_slider.get(), SIGNAL(valueChanged(int)), this, SLOT(changeScale(int)));
	}
}

void MainWindow::changeScale(int value)
{
	const double new_scale = value / 10.0;

	scale(last_scale / new_scale, transformed_image);
	last_scale = new_scale;

	applyTransformations(reference_image, transformed_image);
	ui->dst->setPixmap(QPixmap::fromImage(transformed_image));
}

void MainWindow::setSliders()
{
	if (rot_slider != nullptr)
	{
		rot_slider->blockSignals(true);
		rot_slider->setMinimum(-180000); //-180 to 180 degrees
		rot_slider->setMaximum(180000);
		rot_slider->setValue((int) last_alpha * 1000);
		rot_slider->blockSignals(false);
	}

	if (trans_x_slider != nullptr)
	{
		trans_x_slider->blockSignals(true);
		trans_x_slider->setMinimum(-reference_image.width() * 10);
		trans_x_slider->setMaximum(reference_image.width() * 10);
		trans_x_slider->setValue((int) last_x * 10);
		trans_x_slider->blockSignals(false);
	}

	if (trans_y_slider != nullptr)
	{
		trans_y_slider->blockSignals(true);
		trans_y_slider->setMinimum(-reference_image.height() * 10);
		trans_y_slider->setMaximum(reference_image.height() * 10);
		trans_y_slider->setValue((int) last_y * 10);
		trans_y_slider->blockSignals(false);
	}

	if (scale_slider != nullptr)
	{
		scale_slider->blockSignals(true);
		scale_slider->setMinimum(100);
		scale_slider->setMaximum(5000);
		scale_slider->setValue((int) last_scale * 10);
		scale_slider->blockSignals(false);
	}
}

void MainWindow::clearSliders()
{
	rot_slider = nullptr;
	trans_y_slider = nullptr;
	trans_x_slider = nullptr;
	scale_slider = nullptr;
}
