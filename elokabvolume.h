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

#ifndef ELOKABVOLUME_H
#define ELOKABVOLUME_H



#include <QSystemTrayIcon>

#include "widgetpopup/widgetpopup.h"
#include "widgetvolume.h"
#include "audioengine.h"
class AudioEngine;
class AudioDevice;



#define SETTINGS_MIXER_COMMAND          "mixerCommand"
#define SETTINGS_DEFAULT_MIXER_COMMAND      "pavucontrol"
#define SETTINGS_DEVICE                 "device"
#define SETTINGS_DEFAULT_DEVICE                 0



class ElokabVolume : public QObject
{
    Q_OBJECT

public:
    ElokabVolume( QObject* parent = 0);
void wheelEvent(QWheelEvent* e);
public slots:
void volumeUp();
void volumeDown();
void volumeMute();

private slots:
void showTrayIcon();
    void  createTrayIcon();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void iconChanged(int value);
    void showDialogDevice();
    void showDialogMexer();
    void setAudioEngine(AudioEngine *engine);


    void progressShow();
    void progressHide();



    void togglePopupVolume();
    void realignePopup(bool progress);
    void launchMixer();

void volumHasChanged(int);

    void setupProgressSlider();
    void mixerLineEditChanged(const QString &command);
    void sinkSelectionChanged(const QString &item);
signals:
    void   isVisible(bool v);

protected:

    virtual void settingsChanged();
private:
    AudioEngine *mEngine;

    AudioDevice *mSink;


    WidgetVolume *mWidgetVolume;
    WidgetPopup *mwidgetpopup;
    QString mMixerCommand;

    //***********

    QProgressBar *progressBar;
    QTimer *timeSlide;
QSystemTrayIcon *mTray;
int mCounter;

};



#endif // ELOKABVOLUME_H
