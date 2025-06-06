#ifndef WELCOMEPANEL_H
#define WELCOMEPANEL_H

#include <QDialog>
#include "include/core/general.h"

namespace Ui
{
    class WelcomePanel;
}

class WelcomePanel : public QDialog
{
    Q_OBJECT

public:
    enum LoginMode {NoLogin, LoginAsAdmin, LoginAsUser};

    Q_DISABLE_COPY_MOVE(WelcomePanel)
    explicit WelcomePanel(QWidget *parent = nullptr);
    ~WelcomePanel();

    LoginMode getLoginMode() const;
    IDContainer getSignedkey() const;

public slots:
    void goToSigninPage();
    void goToAccountPage();

    virtual void accept() override;

    void toggleShowLoginPassword(bool value);
    void toggleShowAccountPassword(bool value);

    void rejectCreation();
    void acceptCreation();

private:
    IDContainer signedKey = 0;
    LoginMode mode = LoginMode::NoLogin;
    std::unique_ptr<Ui::WelcomePanel> ui;
};

#endif // WELCOMEPANEL_H
