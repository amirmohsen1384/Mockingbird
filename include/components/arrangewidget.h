#ifndef ARRANGEWIDGET_H
#define ARRANGEWIDGET_H

#include <QWidget>
#include "include/models/proxyplaylistmodel.h"

namespace Ui {
class ArrangeWidget;
}

class ArrangeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ArrangeWidget(QWidget *parent = nullptr);
    ~ArrangeWidget();

public:
    Qt::SortOrder getSortOrder() const;
    ProxyPlaylistModel::Sort getArrangeBase() const;


signals:
    void sortCriteriaChanged();

private:
    Ui::ArrangeWidget *ui;
};

#endif // ARRANGEWIDGET_H
