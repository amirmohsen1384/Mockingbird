#include "include/components/arrangewidget.h"
#include "ui_arrangewidget.h"

ArrangeWidget::ArrangeWidget(QWidget *parent) : QWidget(parent)
{
    ui = std::make_unique<Ui::ArrangeWidget>();
    ui->setupUi(this);

    ui->roleEdit->setItemData(0, QVariant::fromValue(SongProxyModel::ByName));
    ui->roleEdit->setItemData(1, QVariant::fromValue(SongProxyModel::ByReleasedYear));
    ui->roleEdit->setItemData(2, QVariant::fromValue(SongProxyModel::ByArtist));
    ui->roleEdit->setItemData(3, QVariant::fromValue(SongProxyModel::ByGenre));
    connect(ui->roleEdit, &QComboBox::currentIndexChanged, this, &ArrangeWidget::sortCriteriaChanged);
    connect(ui->ascendingEdit, &QCheckBox::checkStateChanged, this, &ArrangeWidget::sortCriteriaChanged);
}

Qt::SortOrder ArrangeWidget::getSortOrder() const
{
    return ui->ascendingEdit->isChecked() ? Qt::AscendingOrder : Qt::DescendingOrder;
}

SongProxyModel::Sort ArrangeWidget::getArrangeBase() const
{
    return ui->roleEdit->currentData().value<SongProxyModel::Sort>();
}
