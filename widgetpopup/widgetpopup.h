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

#ifndef WIDGETPOPUP_H
#define WIDGETPOPUP_H
#include <QWidget>

#include <QFileSystemWatcher>
#include <QSettings>
#include <QHBoxLayout>

class WidgetPopup : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetPopup(QWidget *w=0,QWidget *parent = 0);
    ~WidgetPopup();
    void open(QPoint pos, int s);
   // void setWidget(QWidget *w);

signals:

protected:
    void resizeEvent(QResizeEvent *event);
 //   void paintEvent(QPaintEvent *);

private slots:
 void resizeMe();
 void realign();
 void setupGUi();
private:

      int pMap;

int mMargin;

    QHBoxLayout *hLayout;

    QPoint m_pos;

//ui
    QGridLayout *gridLayoutPopup;
    QWidget *widgetBgrmenu;
    QWidget *widgetBotContener;
    QWidget *widgetBot;
    QWidget *widgetTopContener;
    QWidget *widgetTop;
};

#endif // WIDGETPOPUP_H
