#include "include/components/custom/arrangewidget.h"
#include "ui_arrangewidget.h"

ArrangeWidget::ArrangeWidget(QWidget *parent) : QWidget(parent), ui(new Ui::ArrangeWidget)
{
    ui->setupUi(this);
    ui->roleEdit->setItemData(0, QVariant::fromValue(ProxyPlaylistModel::ByName));
    ui->roleEdit->setItemData(1, QVariant::fromValue(ProxyPlaylistModel::ByReleasedYear));
    ui->roleEdit->setItemData(2, QVariant::fromValue(ProxyPlaylistModel::ByArtist));
    ui->roleEdit->setItemData(3, QVariant::fromValue(ProxyPlaylistModel::ByGenre));
    connect(ui->roleEdit, &QComboBox::currentIndexChanged, this, &ArrangeWidget::sortCriteriaChanged);
    connect(ui->ascendingEdit, &QCheckBox::checkStateChanged, this, &ArrangeWidget::sortCriteriaChanged);
}

ArrangeWidget::~ArrangeWidget()
{
    delete ui;
}

Qt::SortOrder ArrangeWidget::getSortOrder() const
{
    return ui->ascendingEdit->isChecked() ? Qt::AscendingOrder : Qt::DescendingOrder;
}

ProxyPlaylistModel::Sort ArrangeWidget::getArrangeBase() const
{
    return ui->roleEdit->currentData().value<ProxyPlaylistModel::Sort>();
}
