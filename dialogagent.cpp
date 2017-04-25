#include "dialogagent.h"
#include "ui_dialogagent.h"
#include <QDebug>
#include <QTimer>

DialogAgent::DialogAgent(const QString &details,
                         const QString &message,
                         const QString &iconName,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAgent)
{
    ui->setupUi(this);
    // Q_UNUSED(actionId);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    ui->labelMessage->setText(message);
    ui->labelIcon->setPixmap(QIcon::fromTheme(iconName,QIcon::fromTheme("dialog-password")).pixmap(64, 64));
    ui->textEditDetails->setText(details);
    ui->textEditDetails->setVisible(false);

    this->resize(this->width(),this->sizeHint().height());
    this->raise();
    this->activateWindow();
    ui->lineEditPassword->setFocus();
}
void DialogAgent::show()
{
    ui->lineEditPassword->setFocus();
   this->raise();
    this->activateWindow();
    setFocus();
    ui->lineEditPassword->setFocus();
    QDialog::show();
}
DialogAgent::~DialogAgent()
{
    delete ui;
}

void DialogAgent::setIdentities(const QStringList &identities)
{
    foreach (QString identity, identities)
    {
        QString userName=identity;
        //     m_identityMap[userName] = identity;
        ui->comboBoxIdentity->addItem(userName.section(":",-1),identity);
    }
    QString curUser=qgetenv("USER");
    qDebug()<<"curent user :"+curUser;
    int idx=   ui->comboBoxIdentity->findText(curUser);
    if(idx>-1)
        ui->comboBoxIdentity->setCurrentIndex(idx);
}

void DialogAgent::setPrompt(const QString &text, bool echo)
{

    ui->labelPrompt->setText(text);
    ui->lineEditPassword->setEchoMode(echo ? QLineEdit::Normal : QLineEdit::Password);
    ui->lineEditPassword->setFocus();

}

QString DialogAgent::identity()
{
     int idx = ui->comboBoxIdentity->currentIndex();
    return ui->comboBoxIdentity->itemData(idx).toString();


}

QString DialogAgent::response()
{
    return ui->lineEditPassword->text();
}


void DialogAgent::on_toolButton_toggled(bool checked)
{
   QTimer::singleShot(10,this,SLOT(adjustMe()));

}
