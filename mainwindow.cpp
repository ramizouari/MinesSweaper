#include "mainwindow.h"
#include <qapplication.h>
#include <qmessagebox.h>
#include "newgamedialog.h"
#include <qfiledialog.h>
#include <QRegularExpression>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),table(new RMinesTable(20,20,30,this)),languages(detect_languages()),action_group(this)
{
    window = new QFrame(this);
    top_layout = new QHBoxLayout(this);
    main_layout = new QVBoxLayout(this);
    bombs_number = new QLCDNumber(4, this);
    bombs_number->setMaximumHeight(32);
    reset = new QPushButton(this);
    top_layout->addWidget(reset);
    top_layout->addStretch(1);
    top_layout->addWidget(bombs_number);
    bombs_number->display(table->bombs_number());
    bombs_number->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);
    bombs_number->setStyleSheet("color:blue");
    main_layout->addLayout(top_layout);
    main_layout->addWidget(table);
    window->setLayout(main_layout);
    initialize_menu_bar();
    translate();
    setMenuBar(menu_bar);
    setCentralWidget(window);
    connect(table, &RMinesTable::mine_marked, [this](bool F) {
        if (F)
            bombs_number->display(bombs_number->intValue() - 1);
        else bombs_number->display(bombs_number->intValue() + 1);
        });
}

MainWindow::~MainWindow()
{
    delete table;
    delete menu_bar;
    delete game_menu;
    delete about_menu;
    delete bombs_number;
    delete new_game_action;
    delete load_game_action;
    delete save_game_action;
    delete[] languages_actions;
    delete languages_menu;
    delete options_menu;
    delete quit_action;
    delete about_me_action;
    delete about_qt_action;
    delete top_layout;
    delete main_layout;
    delete window;
}

void MainWindow::translate()
{
    game_menu->setTitle(tr("&Game"));
    options_menu->setTitle(tr("&Options"));
    about_menu->setTitle(tr("&About"));
    new_game_action->setText(tr("&New Game"));
    load_game_action->setText(tr("&Load Game"));
    save_game_action->setText(tr("&Save Game"));
    quit_action->setText(tr("&Quit"));
    languages_menu->setTitle(tr("&Languages"));
    about_me_action->setText(tr("About &me"));
    about_qt_action->setText(tr("About &Qt"));
    reset->setText(tr("Reset"));
}

void MainWindow::initialize_menu_bar()
{
    menu_bar = new QMenuBar(this);
    game_menu = new QMenu;
    options_menu = new QMenu;
    about_menu = new QMenu;
    new_game_action = new QAction; 
    load_game_action = new QAction;
    save_game_action = new QAction;
    quit_action = new QAction;
    languages_menu = new QMenu;
    auto n(languages.size());
    languages_actions = new QAction[n];
    for (int i = 0; i < n; i++)
    {
        action_group.addAction(languages_actions + i);
        languages_actions[i].setCheckable(true);
        auto photoPath(qApp->applicationDirPath()+"/translations/icons/" + languages[i] + ".png");
        languages_actions[i].setIcon(QIcon(photoPath));
        languages_actions[i].setText(languages[i]);
        languages_menu->addAction(languages_actions + i);
        connect(languages_actions + i, &QAction::triggered, [this,i]
            {
                auto trName(qApp->applicationDirPath() + "/translations/" + qApp->applicationName()
                    + '_' + languages[i] + ".qm");
                translator.load(trName);
                qApp->installTranslator(&translator);
            });
    }
    action_group.setExclusive(true);
    about_me_action = new QAction;
    about_qt_action = new QAction;
    game_menu->addAction(new_game_action);
    game_menu->addAction(load_game_action);
    game_menu->addAction(save_game_action);
    game_menu->addSeparator();
    game_menu->addAction(quit_action);
    options_menu->addMenu(languages_menu);
    about_menu->addAction(about_me_action);
    about_menu->addAction(about_qt_action);
    menu_bar->addMenu(game_menu);
    menu_bar->addMenu(options_menu);
    menu_bar->addMenu(about_menu);
    connect(quit_action, &QAction::triggered, qApp, &QApplication::quit);
    connect(about_me_action, &QAction::triggered, this, [this]() {
        QMessageBox::information(this,tr("About me") ,tr("MinesSweaper created by TheSilentKiller9"));
        });
    connect(about_qt_action, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(new_game_action, &QAction::triggered, this, &MainWindow::create_game);
    connect(save_game_action, &QAction::triggered, this, &MainWindow::save_game);
    connect(load_game_action, &QAction::triggered, this, &MainWindow::load_game);
}

void MainWindow::create_game()
{
    NewGameDialog D(this);
    connect(&D, &NewGameDialog::accepted, table,&RMinesTable::new_game);
    auto R = D.exec();
    if (R == QDialog::Rejected)
        return;
    table->setEnabled(true);
    bombs_number->display(table->bombs_number());
}

void MainWindow::save_game()
{
    if (!table->isEnabled())
    {
        QMessageBox::warning(this, tr("Lost game"), tr("You cannot save a lost game"));
        return;
    }
    QString str = QFileDialog::getSaveFileName(this,"","","*.mines");
    if (str.isEmpty())
        return;
    table->save_game(str);
}
void MainWindow::load_game()
{
    QString str = QFileDialog::getOpenFileName(this, "", "", "*.mines");
    if (str.isEmpty())
        return;
    table->setEnabled(true);
    table->load_game(str);
    bombs_number->display(table->uncleared_bombs_number());

}

QStringList MainWindow::detect_languages()  const
{
    QDir dir(qApp->applicationDirPath()+"/translations");
    auto L =dir.entryList(QDir::Filter::Files);
    QRegularExpression name("^"+qApp->applicationName()+"_[a-z]+\\.qm$",QRegularExpression::CaseInsensitiveOption);
    auto B = L.filter(name);
    int a;
    for (auto& S : B)
    {
        a = S.indexOf('_');
        S.remove(0, a+1);
        S.chop(3);
    }
    return B;
}

void MainWindow::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        translate();
    }
    else
        QWidget::changeEvent(event);
}
