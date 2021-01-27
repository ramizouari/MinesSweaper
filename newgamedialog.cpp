#include "newgamedialog.h"
#include <qpushbutton.h>
#include "constants.h"

NewGameDialog::NewGameDialog(QWidget *parent)
	: QDialog(parent)
{
	horizontal_spin = new QSpinBox(this);
	horizontal_spin->setMaximum(constants::max_horizontal_grids<int>);
	horizontal_spin->setMinimum(1);
    horizontal_spin->setValue(h);
	vertical_spin = new QSpinBox(this);
	vertical_spin->setMaximum(constants::max_vertical_grids<int>);
	vertical_spin->setMinimum(1);
    vertical_spin->setValue(v);
	bombs_spin = new QSpinBox(this);
	bombs_spin->setMaximum(horizontal_spin->value()*vertical_spin->value());
	bombs_spin->setMinimum(1);
    bombs_spin->setValue(k);
	horizontal_label = new QLabel(tr("Number of horizonatal grids:"), this);
	vertical_label = new QLabel(tr("Number of vertical grids:"), this);
	bombs_label = new QLabel(tr("Number of bombs:"),this);
	form_layout = new QFormLayout(this);
	button_box = new QDialogButtonBox(QDialogButtonBox::StandardButton::Ok | QDialogButtonBox::StandardButton::Cancel);
	form_layout->addRow(horizontal_label,horizontal_spin);
	form_layout->addRow(vertical_label,vertical_spin);
	form_layout->addRow(bombs_label,bombs_spin);
	form_layout->addWidget(button_box);
	void (QSpinBox:: * f)(int) = &QSpinBox::valueChanged;
	connect(vertical_spin, f,[this]()
		{
			auto n = bombs_spin->value(),p(vertical_spin->value() * horizontal_spin->value());
			bombs_spin->setMaximum(p);
			bombs_spin->setValue(std::min(n, p));
		});
    connect(horizontal_spin, f, [this]()
		{
			auto n = bombs_spin->value(), p(vertical_spin->value() * horizontal_spin->value());
			bombs_spin->setMaximum(p);
			bombs_spin->setValue(std::min(n, p));		});
	connect(button_box->button(QDialogButtonBox::StandardButton::Cancel), &QPushButton::clicked, this, &QDialog::reject);
	connect(button_box->button(QDialogButtonBox::StandardButton::Ok), &QPushButton::clicked, [this]() 
		{
			emit accepted(vertical_spin->value(), horizontal_spin->value(), bombs_spin->value());
			accept();
		});
	setLayout(form_layout);
}

NewGameDialog::~NewGameDialog()
{
	delete horizontal_label; 
	delete vertical_label; 
	delete bombs_label;
	delete horizontal_spin;
	delete vertical_spin;
	delete bombs_spin;
	delete form_layout;
	delete button_box;
}

int NewGameDialog::exec()
{
    auto R=QDialog::exec();
    if(R)
    {
        h=horizontal_spin->value();
        v=vertical_spin->value();
        k=bombs_spin->value();
    }
    return R;
}
