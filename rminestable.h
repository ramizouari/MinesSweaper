#ifndef RMINESTABLE_H
#define RMINESTABLE_H

#include <QWidget>
#include <QGridLayout>
#include "rmine.h"
#include "mines_table.h"

class RMinesTable : public QWidget
{
    Q_OBJECT
public:
    explicit RMinesTable(std::uint32_t n,std::uint32_t m,std::uint32_t k,QWidget *parent = nullptr);
    ~RMinesTable();
    int bombs_number() const;
    int uncleared_bombs_number() const;
signals:
    void mine_marked(bool F);
public slots:
    void save_game(const QString& fileName);
    void load_game(const QString& fileName);
    void new_game(std::uint32_t n, std::uint32_t m, std::uint32_t k);
private:
    using BoolMatrix = std::vector<std::vector<bool>>;
    void initialize_mines(const BoolMatrix& opened=BoolMatrix(),
        const BoolMatrix&marked=BoolMatrix());
    void destroy();
    std::vector<std::vector<RMine*>> mines;
    QGridLayout *layout;
    mines_table T;
};

#endif // RMINESTABLE_H
