/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * Razor - a lightweight, Qt based, desktop toolset
 * http://razor-qt.org
 *
 * Copyright: 2011-2012 Razor team
 * Authors:
 *   Petr Vanek <petr@scribus.info>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

/*
              +----------------------------------------------------------+
              |                                                          |
              |  [Icon]  Authentication is needed to run `/bin/bash'     |
              |          as the super user                               |
              |                                                          |
              |          An application is attempting to perform an      |
              |          action that requires privileges. Authentication |
              |          as the super user is required to perform this   |
              |          action.                                         |
              |                                                          |
              |          Password for root: [_________________________]  |
              |                                                          |
              | [V] Details:                                             |
              |  Command: /bin/bash                                      |
              |  Run As:  Super User (root)                              |
              |  Action:  org.freedesktop.policykit.exec                 |
              |  Vendor:  The PolicyKit Project                          |
              |                                                          |
              |                                  [Cancel] [Authenticate] |
              +----------------------------------------------------------+
*/
#include "authenticationagent.h"
#include "notificationutill.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include "polkit-qt5-1/polkitqt1-subject.h"
#include "polkit-qt5-1/polkitqt1-identity.h"
#else
#include "polkit-qt-1/polkitqt1-subject.h"
#include "polkit-qt-1/polkitqt1-identity.h"
#endif

#include <QMessageBox>


#include <QDebug>

AuthenticationAgent::AuthenticationAgent(QObject *parent)
    : PolkitQt1::Agent::Listener(parent),
      m_isRuning(false),
      dialogAgent(nullptr),
      mSession(nullptr)
{

    PolkitQt1::UnixSessionSubject session(getpid());
    if( !registerListener(session, "/org/tawhid/PolicyKit1/AuthenticationAgent")){
        NotificationUtill::Notify("elokab-polkit-agent","elokab",
                                  "authentication","Cannot register authentication agent!",3000);
         qApp->quit();
    }
}

AuthenticationAgent::~AuthenticationAgent()
{
    if (dialogAgent)
    {
        dialogAgent->blockSignals(true);
        dialogAgent->deleteLater();
    }
}


void AuthenticationAgent::initiateAuthentication(const QString &actionId,
                                            const QString &message,
                                            const QString &iconName,
                                            const PolkitQt1::Details &details,
                                            const QString &cookie,
                                            const PolkitQt1::Identity::List &identities,
                                            PolkitQt1::Agent::AsyncResult *result)
{
     Q_UNUSED(details);
    qDebug()<<"actionid:"<<actionId;
    qDebug()<<"message:"       <<message;
    qDebug()<<"iconname:"       <<iconName;

   QString detals=actionId+"\n";
   foreach (QString key, details.keys()) {
        qDebug()<<key<<":"       <<details.lookup(key);
detals+=key+" : "+details.lookup(key)+"\n";

   }
    if (m_isRuning)
    {
        NotificationUtill::Notify("elokab-polkit-agent","elokab","Elokab authentication","Another authentization in progress. Please try it again later",3000);

       // QMessageBox::information(0, tr("PolicyKit Information"), "Another authentization in progress. Please try it again later");
        return;
    }
    m_isRuning = true;
    m_identityMap.clear();

    if (dialogAgent)
    {
        delete dialogAgent;
        dialogAgent = nullptr;
    }
    if(mSession){

        delete mSession;
        mSession=nullptr;
    }
    QStringList listIdIdentifies;
    dialogAgent = new DialogAgent(detals, message, iconName/*, details, lst*/);

    foreach (PolkitQt1::Identity i, identities)
    {
        PolkitQt1::Agent::Session *session;
        session = new PolkitQt1::Agent::Session(i, cookie,result);
        Q_ASSERT(session);
        m_identityMap[i.toString()] = i;
        listIdIdentifies.append(i.toString());
        connect(session, SIGNAL(request(QString, bool)), this, SLOT(setRequest(QString, bool)));
        session->initiate();

    }
    dialogAgent->setIdentities(listIdIdentifies);


  //  execDlg(result,cookie);
    int cod=dialogAgent->exec();
    PolkitQt1::Identity identity =m_identityMap[dialogAgent->identity()];
    mSession = new PolkitQt1::Agent::Session(identity, cookie, result);


    connect(mSession, SIGNAL(showError(QString)), this, SLOT(showError(QString)));
    connect(mSession, SIGNAL(showInfo(QString)), this, SLOT(showInfo(QString)));



    if (cod==QDialog::Accepted){
connect(mSession, SIGNAL(completed(bool)), this, SLOT(completed(bool)));
 mSession->initiate();
        mSession->setResponse(dialogAgent->response());

    } else {

        connect(mSession, SIGNAL(completed(bool)), this, SLOT(canceled(bool)));
         mSession->initiate();

        mSession->setResponse("");

    }

}
void AuthenticationAgent:: setRequest(const QString &request, bool echo)
{
qDebug()<<"***********"<<echo;
    dialogAgent->setPrompt( request, echo);
//    qDebug();
      PolkitQt1::Agent::Session *session = qobject_cast<PolkitQt1::Agent::Session *>(sender());
       Q_ASSERT(session);
      delete session;
}

bool AuthenticationAgent::initiateAuthenticationFinish()
{
    // dunno what are those for...
    m_isRuning = false;
    return true;
}

void AuthenticationAgent::cancelAuthentication()
{

    m_isRuning = false;
}

void AuthenticationAgent::completed(bool gainedAuthorization)
{


    if (!gainedAuthorization)
    {
        QMessageBox::information(nullptr, tr("Authorization Failed"), tr("Authorization failed for some reason"));
    }
    canceled(gainedAuthorization);
}
void AuthenticationAgent::canceled(bool gainedAuthorization)
{
 Q_UNUSED(gainedAuthorization);
    if(mSession){
    mSession->result()->setCompleted();
    delete mSession;
    mSession=nullptr;
    }
    m_isRuning = false;
}


void AuthenticationAgent::showError(const QString &text)
{
    QMessageBox::warning(nullptr, tr("PolicyKit Error"), text);
}

void AuthenticationAgent::showInfo(const QString &text)
{
    QMessageBox::information(nullptr, tr("PolicyKit Information"), text);
}


