#include "volume_adaptor.h"


Volume_adaptor::Volume_adaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

Volume_adaptor::~Volume_adaptor()
{
    // destructor

}


void Volume_adaptor::volumeUp()
{
     QMetaObject::invokeMethod(parent(), "volumeUp");
}

void Volume_adaptor:: volumeDown()
{
     QMetaObject::invokeMethod(parent(), "volumeDown");
}

void Volume_adaptor:: volumeMute()
{
     QMetaObject::invokeMethod(parent(), "volumeMute");
}

