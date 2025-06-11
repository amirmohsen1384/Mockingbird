#ifndef FILTEREDIT_H
#define FILTEREDIT_H

#include <QWidget>
#include "include/core/song.h"
#include "include/models/genremodel.h"

namespace Ui
{
    class FilterEdit;
}

class FilterEdit : public QWidget
{
    Q_OBJECT
public:
    explicit FilterEdit(QWidget *parent = nullptr);

    bool isYearFilteringEnabled() const;
    bool isGenreFilteringEnabled() const;

    int getMinimumYear() const;
    int getMaximumYear() const;
    Song::Genre getGenre() const;

signals:
    void filterChanged();

private:
    GenreModel model;
    std::unique_ptr<Ui::FilterEdit> ui;
};

#endif // FILTEREDIT_H
