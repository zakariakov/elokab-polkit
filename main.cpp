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
#include <QtGui>




#include "authenticationagent.h"


int main(int argc, char *argv[])
{
   //g_type_init();

    QApplication a(argc, argv);

    a.setApplicationName("elokab-polkit-agent");
    a.setApplicationVersion("0.1");
    a.setOrganizationName("elokab");
    a.setQuitOnLastWindowClosed(false);


   /// جلب ترجمة البرنامج من مجلد البرنامج
    /// مسار مجلد البرنامج
    QDir appDir(a.applicationDirPath());
    QString translatPath;
    appDir.cdUp();
    translatPath=  appDir.absolutePath()+"/share/elokab/translations";
    /// جلب اعدادات اللغة
    QSettings globalSetting("elokab","elokabsettings");
    globalSetting.beginGroup("Language");
    QString locale=globalSetting.value("Name","Default").toString();
    globalSetting.endGroup();


    /// اذاكانت اللغة الافتراضية
    if(locale=="Default")
            locale = QLocale::system().name().section("_",0,0);
    /// اللغة الحالية لجميع البرنامج
    QLocale::setDefault(QLocale(locale));
    /// جلب ترجمات كيوتي
    QString translatorFileName = QLatin1String("qt_");
    translatorFileName += locale;
    QTranslator *translatorsys = new QTranslator;
    if (translatorsys->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
            QApplication::installTranslator(translatorsys);
    /// جلب ترجمة البرنامج من مجلد البرنامج
    QTranslator translator;
    translator.load(QString(translatPath+"/"+locale+"/"+a.applicationName()));

    a.installTranslator(&translator);


    AuthenticationAgent agent;

    return a.exec();
}
