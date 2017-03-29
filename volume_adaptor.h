#ifndef VOLUME_ADAPTOR_H
#define VOLUME_ADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>

QT_BEGIN_NAMESPACE


class QString;

QT_END_NAMESPACE

/*
 * Adaptor class for interface org.elokab.session.org
 */

class Volume_adaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.elokab.volume.Interface")
    Q_CLASSINFO("D-Bus Introspection", ""
                                       "  <interface name=\"org.elokab.volume.Interface\">\n"
                                       "    <method name=\"volumeUp\"/>\n"
                                       "    <method name=\"volumeDown\"/>\n"
                                       "    <method name=\"volumeMute\"/>\n"
                                       "  </interface>\n"
                                       "")

//    Q_OBJECT
//    Q_CLASSINFO("D-Bus Interface", "org.elokab.volume.Interface")
//    Q_CLASSINFO("D-Bus Introspection", ""
//                                       "  <interface name=\"org.elokab.volume.Interface\">\n"
//                                       "    <method name=\"setShortcut\">\n"
//                                       "        <arg name=\"shortcut\" type=\"s\" direction=\"in\"/>"
//                                       "    </method>"
//                                       "    <method name=\"reconfigure\">\n"
//                                       "    </method>"
//                                       "    <method name=\"showMenu\">\n"
//                                       "    </method>"
//                                       "    <method name=\"exit\">\n"
//                                       "    </method>"
//                                       "  </interface>\n"
//                                       "")

public:
    Volume_adaptor(QObject *parent);
    virtual ~Volume_adaptor();


public Q_SLOTS: // METHODS
    void volumeUp();
    void volumeDown();
    void volumeMute();


};

#endif
