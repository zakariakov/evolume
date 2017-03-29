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

#ifndef WIDGETVOLUME_H
#define WIDGETVOLUME_H
#include <QWidget>
#include <QToolButton>
#include <QFileSystemWatcher>
#include <QSettings>
#include <QHBoxLayout>
#include <QProgressBar>
class QSlider;

class AudioDevice;
class QLabel;


//namespace Ui {
//class WidgetPopup;
//}

class WidgetVolume : public QWidget
{
        Q_OBJECT
        
public:
        explicit WidgetVolume(QWidget *parent = 0);
        ~WidgetVolume();
        void setProgress( bool progress=false);
        void handleWheelEvent(QWheelEvent *event);

        QSlider *volumeSlider() const { return m_volumeSlider; }

        AudioDevice *device() const { return m_device; }
        void setDevice(AudioDevice *device);
        void setSliderStep(int step);
         void setSliderMax(int max);
void updateStockIcon();
signals:
    void mouseEntered();
    void mouseLeft();

    void volumeChanged(int value);
    void deviceChanged();
    void launchMixer();
    void stockIconChanged(int value);
protected:
    void resizeEvent(QResizeEvent *event);


private slots:
    void handleSliderValueChanged(int value);
    void handleMuteToggleClicked();
    void handleDeviceVolumeChanged(int volume);
    void handleDeviceMuteChanged(bool mute);
//void  loadSettings(QString);

private:
   //  Ui::WidgetPopup *ui;
  //  void realign();

bool m_isCompositing;
    QSlider *m_volumeSlider;
    QToolButton *m_mixerButton;
    QToolButton *m_muteToggleButton;
    QHBoxLayout *hLayout;
//    QProgressBar *m_progressBar;
    QPoint m_pos;
    Qt::Corner m_anchor;
    AudioDevice *m_device;
 QSettings *setting;
  QFileSystemWatcher *m_fileWatcher ;
  int  m_Position;
  int mSize;
};

#endif // WIDGETVOLUME_H
