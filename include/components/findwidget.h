#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include <QWidget>
#include "include/core/playlist.h"

namespace Ui
{
    class FindWidget;
}

class FindWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FindWidget(QWidget *parent = nullptr);
    ~FindWidget();

    QString getText() const;
    Qt::MatchFlag getFlag() const;
    Playlist::Role getFindRole() const;
    Qt::CaseSensitivity getCaseSensitivityMode();

signals:
    void findPropertyChanged();

private:
    Ui::FindWidget *ui;
};

#endif // FINDWIDGET_H
