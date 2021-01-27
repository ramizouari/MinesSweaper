#ifndef RMINE_H
#define RMINE_H

#include <QLabel>
#include <optional>

class RMine : public QLabel
{
    Q_OBJECT
public:
    explicit RMine(QWidget* parent = nullptr);
    explicit RMine(bool _opened,QWidget *parent = nullptr);
    explicit RMine(bool _opened,bool _marked, QWidget* parent = nullptr);
    void mousePressEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent* e) override;
    void resizeEvent(QResizeEvent* event);
    static void load_imgs();
    static const std::optional<QPixmap>& get_flag_img();
    static const std::optional<QPixmap>& get_mine_img();
    bool already_clicked() const;
    void set_marked(bool F);
    bool is_marked() const;
    bool is_opened() const;
    void setText(const QString& S);
    void draw_img(const std::optional<QPixmap>& img, std::string no_img_text);
    static void setColor(const QColor &c,int pos);
    enum class state{None,Mine,Flag,FalseFlag};
    void set_state(state s);
signals:
    void marked_safe(bool);
    void clicked();
    void double_clicked();
public slots:
    void open();
    void disappear();
private:
    static std::optional<QPixmap> mine_img, flag_img;
    static QColor colours[9];
    bool opened;
    bool marked;
    state S;
};
#endif // RMINE_H
