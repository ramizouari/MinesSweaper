#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <qtranslator.h>
#include <qmenubar.h>
#include "rminestable.h"
#include <qpushbutton.h>
#include <qlcdnumber.h>
#include <qactiongroup.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void changeEvent(QEvent* event) override;
private slots:
    void create_game();
    void save_game();
    void load_game();
private:
    void initialize_menu_bar();
    void translate();
    QStringList detect_languages()  const;
      RMinesTable *table;
       QMenuBar *menu_bar;
       QMenu *game_menu,*about_menu,*options_menu,*languages_menu;
       QAction *new_game_action, *load_game_action, *save_game_action,*quit_action,*about_me_action,*about_qt_action,
           *languages_actions;
       QWidget* window;
       QPushButton *reset;
       QLCDNumber *bombs_number;
       QBoxLayout *top_layout,*main_layout;
       QStringList languages;
       QTranslator translator;
       QActionGroup action_group;
};

#endif // MAINWINDOW_H
