#ifndef ARRANGEWIDGET_H
#define ARRANGEWIDGET_H

#include <QWidget>
#include "include/models/songproxymodel.h"

namespace Ui {
class ArrangeWidget;
}

class ArrangeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ArrangeWidget(QWidget *parent = nullptr);

public:
    Qt::SortOrder getSortOrder() const;
    SongProxyModel::Sort getArrangeBase() const;


signals:
    void sortCriteriaChanged();

private:
    std::unique_ptr<Ui::ArrangeWidget> ui;
};

#endif // ARRANGEWIDGET_H
