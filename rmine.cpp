#include "rmine.h"
#include <QMouseEvent>
#include <qcoreapplication.h>
#include <QFile>
#include <iostream>


QColor RMine::colours[9] =
{
    QColor(QColorConstants::Blue),QColor(QColorConstants::DarkGreen),QColor(QColorConstants::Red),
    QColor(QColorConstants::DarkBlue),QColor(QColorConstants::DarkRed),QColor(QColorConstants::DarkCyan),
    QColor(QColorConstants::Magenta),
    QColor(QColorConstants::Gray), QColor(QColorConstants::DarkYellow)
};

std::optional<QPixmap> RMine::mine_img, RMine::flag_img;

RMine::RMine(bool _opened, bool _marked, QWidget* parent ):QLabel(parent),marked(_marked),opened(_opened),S(state::None)
{
    setStyleSheet("font-weight: bold;background-color:#cccccc;");
    setAlignment(Qt::AlignmentFlag::AlignCenter);
    if(opened) 
        disappear();
    else
    {
        //connect(this, &RMine::clicked, this, &RMine::disappear);
        setFrameStyle(QFrame::Shape::Panel|QFrame::Shadow::Raised);
        setMidLineWidth(1);
        setLineWidth(1);
    }
    if (marked)
    {
        S = state::Flag;
        draw_img(RMine::flag_img, "S");
        //setText("S");
    }
}

void RMine::draw_img(const std::optional<QPixmap>& img, std::string no_img_text)
{
    if (img.has_value())
    {
        auto img_scaled(img.value().scaled(.75 * size(), Qt::AspectRatioMode::KeepAspectRatio,
            Qt::TransformationMode::SmoothTransformation));
        setPixmap(img_scaled);
    }
    else setText(QString::fromStdString(no_img_text));
}

RMine::RMine(bool _opened,QWidget* parent) :RMine(_opened,false,parent)
{

}

RMine::RMine(QWidget* parent) : RMine(false, false, parent)
{

}

void RMine::open()
{

}

bool RMine::already_clicked() const
{
    return opened;
}

void RMine::disappear()
{
    opened =true;
    setFrameStyle(QFrame::Shape::NoFrame);
    setLineWidth(0);
}

void RMine::mousePressEvent(QMouseEvent *e)
{
    if (opened)
       return;
    QLabel::mousePressEvent(e);
    if(e->button()==Qt::MouseButton::RightButton)
    {
        if (!marked)
        {
            set_state(state::Flag);
            draw_img(flag_img, "S");
        }
        else { clear(); 
        set_state(state::None);
        }
        marked=!marked;
        emit marked_safe(marked);
    }
    else if(!marked)
        emit clicked();


}

void RMine::mouseDoubleClickEvent(QMouseEvent* e)
{
    if (!opened)
        return;
    QLabel::mouseDoubleClickEvent(e);
    emit double_clicked();
}


void RMine::load_imgs()
{
    auto s1(qApp->applicationDirPath() + "/img/flag.png"), s2(qApp->applicationDirPath() + "/img/mine.png");
    QFile d1(s1), d2(s2);
    if(d1.exists())
        RMine::flag_img= QPixmap(s1);
    if(d2.exists())
        RMine::mine_img = QPixmap(s2);
}

void RMine::set_marked(bool F)
{
    marked=F;
}

bool RMine::is_marked() const
{
    return marked;
}

bool RMine::is_opened() const
{
	return opened;
}

void RMine::setText(const QString& S)
{
    char str = S.toStdString()[0];
    QPalette _palette = palette();
    if (std::isdigit(str))
    {
        auto p = str - '0'-1;
        _palette.setColor(QPalette::ColorRole::WindowText, colours[p]);


    }
    setPalette(_palette);
    QLabel::setText(S);
}

void RMine::setColor(const QColor& c, int pos)
{
    colours[pos] = c;
}

const std::optional<QPixmap>& RMine::get_flag_img()
{
    return RMine::flag_img;
}

const std::optional<QPixmap>& RMine::get_mine_img()
{
    return RMine::mine_img;
}

void RMine::set_state(state s)
{
    S = s;
}

void RMine::resizeEvent(QResizeEvent* event)
{
    QLabel::resizeEvent(event);
    switch (S)
    {
    case RMine::state::Mine:
        draw_img(mine_img, "C");
        break;
    case RMine::state::Flag:
        draw_img(flag_img, "S");
        break;
    case RMine::state::FalseFlag:
        break;
    default:
        break;
    }
}
