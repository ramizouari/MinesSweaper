#include "rminestable.h"
#include <QString>
#include <qcoreapplication.h>
#include <sstream>
#include <fstream>
#include <QPixmap.h>
#include <queue>
#include <iostream>
RMinesTable::RMinesTable(std::uint32_t n,std::uint32_t m,std::uint32_t k,QWidget *parent) : QWidget(parent),
    T(n,m,k),mines(n,std::vector<RMine*>(m))
{
    RMine::load_imgs();
    layout = new QGridLayout(this);
    initialize_mines();
    layout->setSpacing(0);
    setLayout(layout);

}
void RMinesTable::initialize_mines(const BoolMatrix& opened,const BoolMatrix& marked)
{
    for (std::uint32_t i = 0; i < T.vertical_size(); i++)
        for (std::uint32_t j = 0; j < T.horizontal_size(); j++)
        {
            bool O(opened.empty() ? false : opened[i][j]),
                M(marked.empty() ? false : marked[i][j]);
            mines[i][j] = new RMine(O,M,this);
            mines[i][j]->setMinimumSize(12, 12);
            if(O)
            {
                auto U(T.nearby_safe_zones_indices(i, j));
                auto p(T.number_of_nearby_mines(i, j));
                if (p)// if there is at least one neighbour bomb
                {
                    connect(mines[i][j], &RMine::double_clicked, [this, i, j]()
                        {
                            for (auto u : T.nearby_indices(i, j))
                                if (!mines[u.first][u.second]->already_clicked() &&
                                    !mines[u.first][u.second]->is_marked())
                                    emit mines[u.first][u.second]->clicked();
                        });
                }
            }

            connect(mines[i][j], &RMine::marked_safe, [this](bool F)
                {
                    emit mine_marked(F);
                });
            connect(mines[i][j], &RMine::clicked,
                [this, i, j]()
                {
                    if(mines[i][j]->is_marked() || mines[i][j]->is_opened())
                        return;
                    if (T[i][j])
                    {
                        this->setEnabled(false);
                        for (int i = 0; i < T.vertical_size(); i++)
                            for (int j = 0; j < T.horizontal_size(); j++)
                                if (T[i][j] && !mines[i][j]->is_marked())
                                {
                                    mines[i][j]->set_state(RMine::state::Mine);
                                    mines[i][j]->draw_img(RMine::get_mine_img(), "C");
                                    //mines[i][j]->setText("C");
                                }
                                else if (!T[i][j] && mines[i][j]->is_marked())
                                    mines[i][j]->setText("F");
                                else
                                {
                                    std::stringstream S;
                                    std::string str;
                                    auto U(T.nearby_safe_zones_indices(i, j));
                                    auto p(T.number_of_nearby_mines(i, j));
                                    S << p;
                                    S >> str;
                                    mines[i][j]->setText(str.c_str());
                                }
                    }
                    else
                    {
                        std::vector<std::vector<bool>> visited(mines.size(),std::vector<bool>(mines[0].size(),false));
                        std::queue<std::pair<int,int>> Q;

                        std::stringstream S;
                        std::string str;
                        auto U(T.nearby_safe_zones_indices(i, j));
                        auto p(T.number_of_nearby_mines(i, j));
                        S << p;
                        S >> str;
                        mines[i][j]->disappear();
                        connect(mines[i][j],&RMine::double_clicked,
                                [this,i,j]()
                        {
                            for(auto s:T.nearby_indices(i,j))
                                emit  mines[s.first][s.second]->clicked();
                        });
                        if(p)
                        {
                            mines[i][j]->setText(QString(str.c_str()));
                            return;
                        }
                        Q.push({i,j});
                        visited[i][j]=true;
                        while(!Q.empty())
                        {
                            auto w = Q.front();
                            Q.pop();
                            for(const auto &e:T.nearby_safe_zones_indices(w.first,w.second))
                            {
                                if(visited[e.first][e.second] || mines[e.first][e.second]->is_marked() || mines[e.first][e.second]->is_opened())
                                    continue;
                                mines[e.first][e.second]->disappear();
                                visited[e.first][e.second]=true;
                                if(T.is_totally_safe(e.first,e.second))
                                {
                                    Q.push(e);
                                }
                                else
                                {
                                    auto U(T.nearby_safe_zones_indices(e.first, e.second));
                                    auto p(T.number_of_nearby_mines(e.first, e.second));
                                    std::stringstream S;
                                    std::string str;
                                    S << p;
                                    S >> str;
                                    std::cout << str;
                                    mines[e.first][e.second]->setText(QString(str.c_str()));
                                    connect(mines[e.first][e.second],&RMine::double_clicked,
                                            [this,e]()
                                    {
                                        for(auto s:T.nearby_indices(e.first,e.second))
                                            emit  mines[s.first][s.second]->clicked();
                                    });
                                }
                            }
                        }
                    }
                });
        }
    parentWidget()->setUpdatesEnabled(false);
    parentWidget()->blockSignals(true);
    for (std::uint32_t i = 0; i < T.vertical_size(); i++)
        for (std::uint32_t j = 0; j < T.horizontal_size(); j++)
        {
            layout->addWidget(mines[i][j], i, j);
        }
    parentWidget()->blockSignals(false);
    parentWidget()->setUpdatesEnabled(true);
}

 RMinesTable::~RMinesTable()
{
     destroy();
}

 void RMinesTable::destroy()
 {
     for (auto& U : mines)
         for (auto& u : U)
             delete u;
     delete layout;
 }

 void RMinesTable::new_game(std::uint32_t n, std::uint32_t m, std::uint32_t k)
 {
     destroy();
     T.reset(n,m,k);
     mines = std::vector<std::vector<RMine* >> (n, std::vector<RMine*>(m));
     layout = new QGridLayout(this);
     initialize_mines();
     layout->setSpacing(0);
     setLayout(layout);
 }

 void RMinesTable::save_game(const QString& fileName)
 {
     using namespace std;
     ofstream F(fileName.toStdString());
     F << T.vertical_size() << ',' << T.horizontal_size() << std::endl;
     for (int i = 0; i < T.vertical_size(); i++)
         for (int j = 0; j < T.horizontal_size(); j++)
             F << T[i][j] << ' ' << mines[i][j]->is_opened() << ' ' <<mines[i][j]->is_marked()<< ' ';
         
 }

 void RMinesTable::load_game(const QString& fileName)
 {
     using namespace std;
     ifstream F(fileName.toStdString());
     char seperator;
     int n, m;
     bool a,b,c;
     F >> n >> seperator >> m;
     std::vector<std::vector<bool>> marked_table(n, std::vector<bool>(m)), opened_table(marked_table),
         mines_table(marked_table);
     for (int i = 0; i < n; i++)
         for (int j = 0; j < m; j++)
         {
             F >> a >> b >> c;
             mines_table[i][j] = a;
             opened_table[i][j] = b;
             marked_table[i][j] = c;
         }
     destroy();
     T = ::mines_table(mines_table);
     mines = std::vector<std::vector<RMine* >>(n, std::vector<RMine*>(m));
     layout = new QGridLayout(this);
     initialize_mines(opened_table,marked_table);
     for(int i=0;i<n;i++)
         for (int j = 0; j < m; j++)
         {
             if (!opened_table[i][j])
                 continue;
             std::stringstream S;
             std::string str;
             auto U(T.nearby_safe_zones_indices(i, j));
             auto p(T.number_of_nearby_mines(i, j));
             S << p;
             S >> str;
             if (p)
                 mines[i][j]->setText(QString(str.c_str()));
         }
     layout->setSpacing(0);
     setLayout(layout);
 }

 int RMinesTable::bombs_number() const
 {
     int R(0);
     for (int i = 0; i < T.vertical_size(); i++)
         for (int j = 0; j < T.horizontal_size(); j++)
             if (T[i][j])
                 R++;
     return R;
 }

 int RMinesTable::uncleared_bombs_number() const
 {
     int R(0);
     for (int i = 0; i < T.vertical_size(); i++)
         for (int j = 0; j < T.horizontal_size(); j++)
         {
             if (T[i][j])
                 R++;
             if (mines[i][j]->is_marked())
                 R--;
         }
     return R;
 }
