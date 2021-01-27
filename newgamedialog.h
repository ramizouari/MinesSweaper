#pragma once

#include <qdialog.h>
#include <qlabel.h>
#include <QFormLayout.h>
#include <qboxlayout.h>
#include <qspinbox.h>
#include <QDialogButtonBox.h>

class NewGameDialog : public QDialog
{
	Q_OBJECT

public:
	NewGameDialog(QWidget *parent);
	~NewGameDialog();
public slots:
	int exec() override;
signals:
	void accepted(int v, int h, int k);
private:
	QLabel *horizontal_label, * vertical_label, *bombs_label;
	QSpinBox *horizontal_spin, * vertical_spin, * bombs_spin;
	QFormLayout *form_layout;
	QDialogButtonBox* button_box;
    inline static int v = 30,h =20,k=50;
};
