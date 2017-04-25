#ifndef DIALOGAGENT_H
#define DIALOGAGENT_H

#include <QDialog>


namespace Ui {
class DialogAgent;
}
/**
 * @brief The DialogAgent class
 */
class DialogAgent : public QDialog
{
    Q_OBJECT
    
public:
    /**
     * @brief DialogAgent
     * @param details معلومات عن التطبيق
     * @param message الرسالة التي يطلبها النطبيق
     * @param iconName اسم الايقونة
     * @param parent
     */
    explicit DialogAgent(const QString &details, const QString &message,
                          const QString &iconName, QWidget *parent = 0);
    ~DialogAgent();
void show();
/**
     * @brief setIdentities
     * @param identities قائمة باسماء المستخدمين
     */
    void setIdentities(const QStringList &identities);
    /**
     * @brief setPrompt
     * @param text انص
     * @param echo
     */
    void setPrompt(const QString &text, bool echo);
    /**
     * @brief identity
     * @return
     */
    QString identity();
    /**
     * @brief response
     * @return
     */
    QString response();

private slots:
    void on_toolButton_toggled(bool checked);

    void adjustMe(){this->adjustSize(); }
private:
    Ui::DialogAgent *ui;
};

#endif // DIALOGAGENT_H
