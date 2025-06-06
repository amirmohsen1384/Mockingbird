#include "include/components/filteredit.h"
#include "include/core/playlist.h"
#include "ui_filteredit.h"

FilterEdit::FilterEdit(QWidget *parent) : QWidget(parent)
{
    ui = std::make_unique<Ui::FilterEdit>();
    ui->setupUi(this);

    ui->genreEdit->setModel(&model);

    ui->startYearEdit->setMinimum(_min_year);
    ui->endYearEdit->setMaximum(QDate::currentDate().year());

    connect(ui->endYearEdit, &QSpinBox::valueChanged, ui->startYearEdit, &QSpinBox::setMaximum);
    connect(ui->startYearEdit, &QSpinBox::valueChanged, ui->endYearEdit, &QSpinBox::setMinimum);
    connect(ui->genreEdit, &QComboBox::currentIndexChanged, this, &FilterEdit::filterChanged);
    connect(ui->startYearEdit, &QSpinBox::valueChanged, this, &FilterEdit::filterChanged);
    connect(ui->endYearEdit, &QSpinBox::valueChanged, this, &FilterEdit::filterChanged);
    connect(ui->yearFilterGroup, &QGroupBox::toggled, this, &FilterEdit::filterChanged);
    connect(ui->genreGroup, &QGroupBox::toggled, this, &FilterEdit::filterChanged);

    ui->endYearEdit->setValue(QDate::currentDate().year());
}

FilterEdit::~FilterEdit() {}

bool FilterEdit::isYearFilteringEnabled() const
{
    return ui->yearFilterGroup->isChecked();
}

bool FilterEdit::isGenreFilteringEnabled() const
{
    return ui->genreGroup->isChecked();
}

int FilterEdit::getMinimumYear() const
{
    return ui->startYearEdit->value();
}

int FilterEdit::getMaximumYear() const
{
    return ui->endYearEdit->value();
}

Song::Genre FilterEdit::getGenre() const
{
    return ui->genreEdit->currentData(Qt::UserRole).value<Song::Genre>();
}
