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

#include "widgetvolume.h"
#include "notificationutill.h"
#include "volume_adaptor.h"
#include "audiodevice.h"
#include <QSlider>
#include <QDebug>


WidgetVolume::WidgetVolume(QWidget *parent) :
        QWidget(parent),
        m_pos(0,0),
        m_anchor(Qt::TopLeftCorner),
        m_device(0)
{

    m_mixerButton = new QToolButton(this);

    m_mixerButton->setToolTip(tr("Launch mixer"));
    m_mixerButton->setText(tr("Mixer"));
    //        m_mixerButton->setText(QString("<a href=\"#\">%1</a>").arg(tr("Mixer")));
    m_mixerButton->setIconSize(QSize(22,22));
    m_mixerButton->setIcon(QIcon::fromTheme( "mixer-master",QIcon(":/icons/mixer-master")));
    m_mixerButton->setAutoRaise(true);

    m_volumeSlider = new QSlider(Qt::Horizontal);
    m_volumeSlider->setTickPosition(QSlider::TicksBothSides);
    m_volumeSlider->setTickInterval(10);

    m_muteToggleButton = new QToolButton(this);
    m_muteToggleButton->setIconSize(QSize(48,48));
    m_muteToggleButton->setIcon(QIcon::fromTheme( "audio-volume-muted",QIcon(":/icons/audio-volume-muted")));
    m_muteToggleButton->setCheckable(true);
    m_muteToggleButton->setAutoRaise(true);


    hLayout = new QHBoxLayout(this);
    hLayout->setSpacing(3);
    hLayout->setMargin(0);

    hLayout->addWidget(m_mixerButton);
    hLayout->addWidget(m_volumeSlider);
//    hLayout->addWidget(m_progressBar);
    hLayout->addWidget(m_muteToggleButton);

    connect(m_mixerButton, SIGNAL(clicked()), this, SIGNAL(launchMixer()));
    connect(m_volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(handleSliderValueChanged(int)));
    connect(m_muteToggleButton, SIGNAL(clicked()), this, SLOT(handleMuteToggleClicked()));
    setMinimumSize(250,50);


}

WidgetVolume::~WidgetVolume()
{

}


void WidgetVolume::handleSliderValueChanged(int value)
{
    if (!m_device)
        return;

    m_device->setVolume(value);

}

void WidgetVolume::handleMuteToggleClicked()
{
    if (!m_device)
        return;

    m_device->toggleMute();
}

void WidgetVolume::handleDeviceVolumeChanged(int volume)
{
    emit volumeChanged(volume);
    m_volumeSlider->setValue(volume);
//    m_progressBar->setValue(volume);

    updateStockIcon();
}

void WidgetVolume::handleDeviceMuteChanged(bool mute)
{
    m_muteToggleButton->setChecked(mute);
    updateStockIcon();
}

void WidgetVolume::updateStockIcon()
{
    if (!m_device)
        return;

    QString iconName;
    if (m_device->volume() <= 0 || m_device->mute())
        iconName = "audio-volume-muted";
    else if (m_device->volume() <= 33)
        iconName = "audio-volume-low";
    else if (m_device->volume() <= 66)
        iconName = "audio-volume-medium";
    else
        iconName = "audio-volume-high";

    m_muteToggleButton->setIcon(QIcon::fromTheme(iconName,QIcon(":/icons/"+iconName)));
    emit stockIconChanged(m_device->volume());

//    if(NotificationUtill::dbusFreedesktopIsValid())
//        NotificationUtill::Notify(this->objectName(),
//                                  iconName,QString(),QString::number(m_device->volume()),   5000);

}

void WidgetVolume::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //realign();
}

void WidgetVolume::setProgress(bool progress)
{
    if(progress){
       // hLayout->removeWidget(m_volumeSlider);
        m_volumeSlider->setVisible(false);
        m_mixerButton->setVisible(false);
            //    hLayout->addWidget(m_progressBar);
//        m_progressBar->setVisible(true);
    }else{
      //  hLayout->removeWidget(m_progressBar);
//         m_progressBar->setVisible(false);
       // hLayout->addWidget(m_volumeSlider);
        m_volumeSlider->setVisible(true);
         m_mixerButton->setVisible(true);
    }

}

void WidgetVolume::handleWheelEvent(QWheelEvent *event)
{
   m_volumeSlider->event(reinterpret_cast<QEvent*>(event));
}

void WidgetVolume::setDevice(AudioDevice *device)
{
    if (device == m_device)
        return;

    // disconnect old device
    if (m_device)
        disconnect(m_device);

    m_device = device;

    if (m_device) {
        m_volumeSlider->setValue(m_device->volume());
//        m_progressBar->setValue(m_device->volume());
        m_muteToggleButton->setChecked(m_device->mute());

        connect(m_device, SIGNAL(volumeChanged(int)), this, SLOT(handleDeviceVolumeChanged(int)));
        connect(m_device, SIGNAL(muteChanged(bool)), this, SLOT(handleDeviceMuteChanged(bool)));
    }

    updateStockIcon();
    emit deviceChanged();
}

void WidgetVolume::setSliderStep(int step)
{
    m_volumeSlider->setSingleStep(step);
    m_volumeSlider->setPageStep(step * 10);
}
void WidgetVolume::setSliderMax(int max)
{
    m_volumeSlider->setMaximum(max);
//m_progressBar->setMaximum(max);
}
