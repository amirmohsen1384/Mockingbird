#include "include/components/custom/findwidget.h"
#include "ui_findwidget.h"

FindWidget::FindWidget(QWidget *parent) : QWidget(parent), ui(new Ui::FindWidget)
{
    ui->setupUi(this);

    ui->searchModeEdit->setItemData(1, QVariant::fromValue(Qt::MatchFlag::MatchEndsWith));
    ui->searchModeEdit->setItemData(2, QVariant::fromValue(Qt::MatchFlag::MatchContains));
    ui->searchModeEdit->setItemData(0, QVariant::fromValue(Qt::MatchFlag::MatchStartsWith));

    connect(ui->searchModeEdit, &QComboBox::currentIndexChanged, this, &FindWidget::findPropertyChanged);
    connect(ui->caseSensitivityEdit, &QCheckBox::clicked, this, &FindWidget::findPropertyChanged);
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &FindWidget::findPropertyChanged);
    connect(ui->nameEdit, &QRadioButton::clicked, this, &FindWidget::findPropertyChanged);
}

FindWidget::~FindWidget()
{
    delete ui;
}

QString FindWidget::getText() const
{
    return ui->searchEdit->text();
}

Qt::MatchFlag FindWidget::getFlag() const
{
    return ui->searchModeEdit->currentData().value<Qt::MatchFlag>();
}

FindWidget::Base FindWidget::getSearchBase() const
{
    return ui->nameEdit->isChecked() ? FindWidget::Name : FindWidget::Artist;
}

Qt::CaseSensitivity FindWidget::getCaseSensitivityMode()
{
    return ui->caseSensitivityEdit->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
}
