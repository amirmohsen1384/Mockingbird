#ifndef USERPLAYLISTEDITOR_H
#define USERPLAYLISTEDITOR_H

#include "include/dialogs/editor/abstractplaylisteditor.h"

class UserPlaylistEditor : public AbstractPlaylistEditor
{
    Q_OBJECT
public:
    UserPlaylistEditor(QWidget *parent = nullptr);

    bool isHeaderDataEditable() const;

public slots:
    virtual void addSong() override;
    virtual void performSong(const QModelIndex &index) override;

    void setHeaderDataEditable(bool value);

signals:
    void headerDataEditableChanged(bool state);

private:
    bool state;
};

#endif // USERPLAYLISTEDITOR_H
