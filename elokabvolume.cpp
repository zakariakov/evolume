

#include "elokabvolume.h"
#include <QInputDialog>
#include <QMenu>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QProcess>
#include <QPainter>


#include "audiodevice.h"
#include "alsaengine.h"




ElokabVolume::ElokabVolume(QObject *parent):
    QObject(parent),
    mEngine(0),
    mSink(0),
    mCounter(0)
{
   mTray=new QSystemTrayIcon;
 qApp->setQuitOnLastWindowClosed(false);
    createTrayIcon();
    //connect(trayIcon, &QSystemTrayIcon::messageClicked, this, &ElokabVolume::messageClicked);
    connect(mTray, &QSystemTrayIcon::activated, this, &ElokabVolume::iconActivated);

    setupProgressSlider();

    //setScaledContents(true);
    //********************************************


    mWidgetVolume=new WidgetVolume();

    mwidgetpopup=new WidgetPopup(mWidgetVolume);
    connect(mWidgetVolume, SIGNAL(launchMixer()), this, SLOT(launchMixer()));
    connect(mWidgetVolume, SIGNAL(stockIconChanged(int)), this, SLOT(iconChanged(int)));
connect(mWidgetVolume, SIGNAL(volumeChanged(int)), this, SLOT(volumHasChanged(int)));

    if (!mEngine)
        setAudioEngine(new AlsaEngine(this));

    //**********************************
    settingsChanged();
showTrayIcon();


}
void ElokabVolume::createTrayIcon()
{
    QMenu *menu=new QMenu;
    QAction *a;
    Q_UNUSED(a);

    a=menu->addAction(QIcon::fromTheme(""),tr("Mixer"),this,SLOT(launchMixer()));
    a=menu->addSeparator();
    a=menu->addAction(QIcon::fromTheme(""),tr("Mute/Unmute"),this,SLOT(volumeMute()));
    a=menu->addSeparator();
    a=menu->addAction(QIcon::fromTheme(""),tr("Select Device"),this,SLOT(showDialogDevice()));
    a=menu->addAction(QIcon::fromTheme(""),tr("Select Mixer"),this,SLOT(showDialogMexer()));
    a=menu->addSeparator();
    a=menu->addAction(QIcon::fromTheme(""),tr("Exit"),qApp,SLOT(quit()));


    mTray->setContextMenu(menu);
    QIcon icon=QIcon::fromTheme("audio-volume-high",
                                QIcon::fromTheme("audio-volume-high-panel",
                                                 QIcon(":/icons/audio-volume-high")));

   mTray->setIcon(icon);
}

void ElokabVolume::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        togglePopupVolume();

    case QSystemTrayIcon::DoubleClick:

        break;
    case QSystemTrayIcon::MiddleClick:
        // showMessage();
        break;
    default:
        ;
    }
}




void ElokabVolume::showDialogDevice()
{
    QStringList items;

    if (mEngine){
        foreach (const AudioDevice *dev, mEngine->sinks()) {
            items.append(dev->description());

        }



        QInputDialog *dlg=new QInputDialog;
        dlg->setWindowTitle( tr("Device:"));
        dlg->setLabelText( tr("select device:"));
        dlg->setComboBoxItems(items);
        dlg->setComboBoxEditable(false);//
        connect(dlg,SIGNAL(textValueSelected(QString)),this,SLOT( sinkSelectionChanged(QString)));
        dlg->show();

    }

}
void ElokabVolume::showDialogMexer()
{

    QInputDialog *dlg=new QInputDialog;
    dlg->setWindowTitle( tr("Mixer:"));
    dlg->setLabelText( tr("Enter Mixer name:"));
    dlg->setTextValue(mMixerCommand);
    dlg->setTextEchoMode(QLineEdit::Normal);
    connect(dlg,SIGNAL(textValueSelected(QString)),this,SLOT( mixerLineEditChanged(QString)));
    dlg->show();
}


void ElokabVolume::setupProgressSlider()
{


    QString style=(QString::fromUtf8("QProgressBar {\n"
                                     "	background-color: rgba(188, 188, 188, 100);\n"
                                     "     border: 2px solid rgba(255, 255, 255, 195);\n"
                                     "	color: rgb(0, 0, 0);\n"
                                     "     border-radius: 5px;\n"
                                     "text-align: center;\n"
                                     " }\n"
                                     "\n"
                                     "QProgressBar::chunk {\n"
                                     "   \n"
                                     "	background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(255, 255, 255, 110), stop:0.488636 rgba(255, 255, 255, 237), stop:1 rgba(255, 255, 255, 110));\n"
                                     "\n"
                                     "     width: 10px;\n"
                                     "     margin: 1.5px;\n"
                                     " }"));
    progressBar = new QProgressBar;
    progressBar->resize(272, 41);
    progressBar->setObjectName(QString::fromUtf8("progressBar"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
    progressBar->setSizePolicy(sizePolicy);
    QFont font;
    font.setPointSize(14);
    font.setBold(true);
    font.setWeight(75);
    progressBar->setFont(font);
    progressBar->setValue(50);
    progressBar->setStyleSheet(style);
    progressBar->setWindowFlags ( Qt::Popup | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint | Qt::X11BypassWindowManagerHint);
    progressBar->setAttribute(Qt::WA_TranslucentBackground,true);
    QRect screen = QApplication::desktop()->availableGeometry();
    int x=screen.width()/2-progressBar->width()/2;
    int y=screen.height()/2-progressBar->height()/2;
    progressBar->move(x,y);
    timeSlide = new QTimer(this);
    connect(timeSlide, SIGNAL(timeout()), this, SLOT(progressHide()));

}


void ElokabVolume::mixerLineEditChanged(const QString &command)
{
    qDebug()<<"command :"<<command;
    QSettings settings("elokab","volume");
    settings.beginGroup("Main");
    settings.setValue(SETTINGS_MIXER_COMMAND, command);
    settings.endGroup();
    settingsChanged();
}
void ElokabVolume::sinkSelectionChanged(const QString &item)
{
    int index=0;
    qDebug()<<"item device selected :"<<item;
    if (mEngine)
        foreach (const AudioDevice *dev, mEngine->sinks()) {
            if(item==dev->description())
                index= dev->index();

        }

    QSettings settings("elokab","volume");
    settings.beginGroup("Main");
    settings.setValue(SETTINGS_DEVICE, index);
    settings.endGroup();
    settingsChanged();
}

//! تشغيل مخلط خارجي
void ElokabVolume::launchMixer()
{
    //  QProcess::startDetached(m_mixerCommand);
    QProcess::startDetached(mMixerCommand);
}
//! تغير الايقونات
void ElokabVolume::iconChanged(int value)
{
   QString iconName;

    if (value <= 0)
        iconName = "audio-volume-muted";
    else if (value <= 33)
        iconName = "audio-volume-low";
   else if (value <= 66)
       iconName = "audio-volume-medium";
   else
         iconName = "audio-volume-high";

   QIcon icon=QIcon::fromTheme(iconName,QIcon::fromTheme(iconName+"-panel",QIcon(":/icons/"+iconName)));

   mTray->setIcon(icon);

}

/*************************************
 **********نافذة تغيير الصوت************
 *************************************/
//! اظهار اخفاء نافذة تغيير الصوت
void ElokabVolume::togglePopupVolume()
{
    if (mwidgetpopup->isVisible())
        mwidgetpopup->hide();
    else
        realignePopup(false);

}
//!  تموضع نافذة تغيير الصوت
void ElokabVolume::realignePopup(bool progress)
{
    if (mwidgetpopup->isVisible())
        return;

    //هنا جلب اعدادت العقاب بانال

    mWidgetVolume->updateGeometry();

    mWidgetVolume->setProgress(progress);

   //TODO FIX THIS
   int x=mTray->geometry().x();
   int y=mTray->geometry().height();
   int s=mTray->geometry().height();
    mwidgetpopup->open(QPoint(x,y), s);



}

//  نحديد معامل الصوت المستخدم
void ElokabVolume::setAudioEngine(AudioEngine *engine)
{
    if (mEngine) {
        if (mEngine->backendName() == engine->backendName())
            return;

        mWidgetVolume->setDevice(0);

        disconnect(mEngine, 0, 0, 0);
        delete mEngine;
        mEngine = 0;
    }

    mEngine = engine;

}

//! تغير الاعدادات
void ElokabVolume::settingsChanged()
{

    int mSinkIndex=0;
    QSettings settings("elokab","volume");

    settings.beginGroup("Main");
    mMixerCommand=(settings.value(SETTINGS_MIXER_COMMAND, SETTINGS_DEFAULT_MIXER_COMMAND).toString());
    mSinkIndex = settings.value(SETTINGS_DEVICE, SETTINGS_DEFAULT_DEVICE).toInt();
    settings.endGroup();

    if (mEngine && mEngine->sinks().count() > 0) {
        mSinkIndex = qBound(0, mSinkIndex, mEngine->sinks().count()-1);

        mSink = mEngine->sinks().at(mSinkIndex);
        mWidgetVolume->setDevice(mSink);
        mWidgetVolume->setSliderMax(mEngine->volumeMax(mSink));
        //    progressBar->setMaximum(mEngine->volumeMax(mSink));
        mEngine->setIgnoreMaxVolume(true);
    }
}

/*********************************************************
 *************************لوحة المفاتيح*******************
 *********************************************************/
//! عند النقرعلى لوحة المفاتيح للزيادة الصوت
void ElokabVolume::volumeUp()
{

    if (mSink)
    {
        mSink->setVolume(mSink->volume() + 3);

        progressShow();
    }
}
//! عمد النقرعلى لوحة المفاتيح لنقصان الصوت
void ElokabVolume::volumeDown()
{

    if (mSink)
    {

        mSink->setVolume(mSink->volume() - 3);

        progressShow();
    }
}
//! عمد النقرعلى لوحة المفاتيح لكتم الصوت
void ElokabVolume::volumeMute()
{
    if (mSink){

        mSink->toggleMute();
        progressShow();
    }
}
//! عرض مؤشر تغير حجم الصوت على الشاشة
void ElokabVolume::progressShow()
{

    timeSlide->stop();

    realignePopup(true);

    timeSlide->start(5000);


}
//! اخفاء مؤشر تغير حجم الصوت
void ElokabVolume::progressHide()
{

    mwidgetpopup->hide();
    timeSlide->stop();
}
/*********************************
 *********************************/
//! تغيير حجم الصوت بواسطة عجلة الفأرة
void ElokabVolume::wheelEvent(QWheelEvent* e)
{


    int delta = e->delta() < 0 ? 3 : -3;

    int volume =mSink->volume() - delta;

    if (!mwidgetpopup->isVisible())
        progressShow();

    mSink->setVolume( volume);

}

void ElokabVolume::volumHasChanged(int)
{

    if (!mwidgetpopup->isVisible())
        progressShow();

}

void ElokabVolume::showTrayIcon()
{
     mCounter++;
     if(mCounter>=60)
         qApp->quit();//TODO add message here

    if(QSystemTrayIcon::isSystemTrayAvailable())
        mTray->show();
    else
        QTimer::singleShot(5000,this,SLOT(showTrayIcon()));

}

