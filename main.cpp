/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)GPL3+
 *
 * elokab - a lightweight, Qt based, desktop toolset
 * https://sourceforge.net/project/elokab/
 *
 * Copyright: 2013-2014 elokab team
 * Authors:
 *   Abou Zakaria <yahiaui@gmail.com>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
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

#include "volume_adaptor.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QDir>
#include <QTranslator>

#include <QDBusConnection>
#include <QDebug>
#include "elokabvolume.h"
void helpMe()
{
    printf("Usage: evolume [OPTION]\n");
    puts("OPTION:\n");
    puts(" -h  --help                 Print this help.\n");
    puts(" -u  --volume-up            curent volume ++.\n");
    puts(" -d, --volume-down          curent volume --\n");
    puts(" -m, --mute-unmute          toggle mute unmute\n");

}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("elokab-volume");
    a.setApplicationVersion("0.1");
    a.setOrganizationName("elokab");

    //---------------------------------------------------------------QDBusConnection
    QDBusConnection connection = QDBusConnection::sessionBus();
    if (! QDBusConnection::sessionBus().registerService("org.elokab.volume"))
    {

        QDBusInterface dbus("org.elokab.volume",
                            "/org/elokab/volume",
                            "org.elokab.volume.Interface");

        if (!dbus.isValid()) {
            qDebug() << "QDBusInterface is not valid!";
            return 1;
        }
        QStringList args = a.arguments();

        if(args.count()>1)
        {

            QString arg = args.at(1);
            if (arg == "-h" || arg == "--help" ) {helpMe();return 0; }
            else if (arg == "-u" || arg == "--volume-up" )  {dbus.call("volumeUp");return 0;}
            else if (arg == "-d" || arg == "--volume-down ") {dbus.call("volumeDown");return 0;}
            else if (arg == "-m" || arg == "--mute-unmute " ) {
                QDBusMessage msg = dbus.call("volumeMute");
                 QString response = msg.arguments().first().toString();
                 qDebug() << "dbus:volumeMute"  << "=" << response;
                return 0;}

            else { qWarning() << "echo Unknown option: " << args;  helpMe(); return 0; }

        }
        return 0;
    }

    //--------------------------------------------------------------------LANGUAGE
    /// مسار مجلد البرنامج
    QDir appDir(a.applicationDirPath());
    QString appPath;
    appDir.cdUp();
    appPath=  appDir.absolutePath()+"/share/elokab/elokab-volume";
    /// جلب اعدادات اللغة
    QSettings globalSetting("elokab","elokabsettings");
    globalSetting.beginGroup("Language");
    QString locale=globalSetting.value("Name","Default").toString();
    globalSetting.endGroup();
    globalSetting.beginGroup("Themes");
    QIcon::setThemeName(globalSetting.value("iconsTheme",QIcon::themeName()).toString());
    globalSetting.endGroup();
    a.setWindowIcon(QIcon::fromTheme("audio-volume-high",QIcon(":/icons/audio-volume-high")));


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
    translator.load(QString(appPath+"/languages/elokab-volume_"+locale+".qm"));

    a.installTranslator(&translator);

     ElokabVolume w;
     new Volume_adaptor(&w);

 connection.registerObject("/org/elokab/volume", &w);
    //w.show();

    return a.exec();
}
