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

#ifndef POLICYKITAGENT_H
#define POLICYKITAGENT_H

#define POLKIT_AGENT_I_KNOW_API_IS_SUBJECT_TO_CHANGE 1
#include "dialogagent.h"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include "polkit-qt5-1/polkitqt1-agent-session.h"
#include "polkit-qt5-1/polkitqt1-identity.h"
#include <polkit-qt5-1/polkitqt1-details.h>
#include <polkit-qt5-1/polkitqt1-agent-listener.h>
#else
#include "polkit-qt-1/polkitqt1-agent-session.h"
#include "polkit-qt-1/polkitqt1-identity.h"
#include <polkit-qt-1/polkitqt1-details.h>
#include <polkit-qt-1/polkitqt1-agent-listener.h>
#endif


#include <QApplication>
#include <QHash>


class PolicykitAgentGUI;
/**
 * @brief The AuthenticationAgent class
 */
class AuthenticationAgent : public PolkitQt1::Agent::Listener
{
    Q_OBJECT

public:
    AuthenticationAgent(QObject *parent = nullptr);
    ~AuthenticationAgent();

public slots:
    /**
     * @brief initiateAuthentication
     * @param actionId
     * @param message
     * @param iconName
     * @param details
     * @param cookie
     * @param identities
     * @param result
     */
    void initiateAuthentication(const QString &actionId,
                                const QString &message,
                                const QString &iconName,
                                const PolkitQt1::Details &details,
                                const QString &cookie,
                                const PolkitQt1::Identity::List &identities,
                                PolkitQt1::Agent::AsyncResult *result);
    /**
     * @brief initiateAuthenticationFinish
     * @return
     */
    bool initiateAuthenticationFinish();
    /**
     * @brief cancelAuthentication
     */
    void cancelAuthentication();
    /**
     * @brief setRequest
     * @param request
     * @param echo
     */
    void setRequest(const QString &request, bool echo);
    /**
     * @brief completed
     * @param gainedAuthorization
     */
    void completed(bool gainedAuthorization);
    /**
       * @brief canceled
       * @param gainedAuthorization
       */
      void canceled(bool gainedAuthorization);
      /**
     * @brief showError
     * @param text
     */
    void showError(const QString &text);
    /**
     * @brief showInfo
     * @param text
     */
    void showInfo(const QString &text);

private:
    bool m_isRuning;
     DialogAgent * dialogAgent;
    PolkitQt1::Agent::Session * mSession;

    //  QHash<PolkitQt1::Agent::Session*,PolkitQt1::Identity> m_SessionIdentity;
    QHash<QString,PolkitQt1::Identity> m_identityMap;
};

#endif
