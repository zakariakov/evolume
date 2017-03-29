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

#include "widgetpopup.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QSettings>
#include <QPainter>
#include <QX11Info>
#include <QTimer>
WidgetPopup::WidgetPopup(QWidget *w, QWidget *parent) :
    QWidget(parent,  Qt::Popup | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint | Qt::X11BypassWindowManagerHint),
    m_pos(0,0)
{


    setupGUi();
mMargin=7;
    hLayout->addWidget(w);


    QGraphicsDropShadowEffect *fx = new QGraphicsDropShadowEffect();
  //  if(this->isRightToLeft())
        fx->setOffset(0);


        fx->setBlurRadius(14);
//QColor color=this->palette().text().color();
    fx->setColor(QColor(0, 0, 0, 200));

    // this->setPalette(Qt::transparent);
    this->setAttribute(Qt::WA_TranslucentBackground,true);

    setGraphicsEffect(fx);

}


void WidgetPopup::setupGUi()
{
    setObjectName(QString::fromUtf8("WidgetPopup"));
    resize(76, 50);
    gridLayoutPopup = new QGridLayout(this);
    gridLayoutPopup->setSpacing(0);
    gridLayoutPopup->setObjectName(QString::fromUtf8("gridLayoutPopup"));
    gridLayoutPopup->setContentsMargins(6, 6, 6, 6);
    widgetBgrmenu = new QWidget(this);
    widgetBgrmenu->setObjectName(QString::fromUtf8("widgetBgrmenu"));


    gridLayoutPopup->addWidget(widgetBgrmenu, 1, 0, 1, 2);

    widgetBotContener = new QWidget(this);
    widgetBotContener->setObjectName(QString::fromUtf8("widgetBotContener"));
    widgetBotContener->setMinimumSize(QSize(0, 13));
    widgetBotContener->setMaximumSize(QSize(QWIDGETSIZE_MAX, 13));
    widgetBot = new QWidget(widgetBotContener);
    widgetBot->setObjectName(QString::fromUtf8("widgetBot"));
    widgetBot->setGeometry(QRect(0, 0, 45, 13));
    widgetBot->setMinimumSize(QSize(45, 13));
    widgetBot->setMaximumSize(QSize(45, 13));


    gridLayoutPopup->addWidget(widgetBotContener, 2, 0, 1, 2);

    widgetTopContener = new QWidget(this);
    widgetTopContener->setObjectName(QString::fromUtf8("widgetTopContener"));
    widgetTopContener->setMinimumSize(QSize(0, 10));
    widgetTopContener->setMaximumSize(QSize(QWIDGETSIZE_MAX, 10));
    widgetTop = new QWidget(widgetTopContener);
    widgetTop->setObjectName(QString::fromUtf8("widgetTop"));
    widgetTop->setGeometry(QRect(0, 0, 45, 13));
    widgetTop->setMinimumSize(QSize(45, 13));
    widgetTop->setMaximumSize(QSize(45, 13));


    gridLayoutPopup->addWidget(widgetTopContener, 0, 0, 1, 2);


    hLayout = new QHBoxLayout(widgetBgrmenu);
    hLayout->setSpacing(3);
    hLayout->setMargin(9);
QColor color=this->palette().window().color();
color.setAlpha(200);
  setStyleSheet(QString("QWidget #widgetBgrmenu{ background-color: rgba(%1, %2, %3 ,%4); border-radius:10px;}")
                .arg(color.red()).arg(color.green())
                .arg(color.blue()).arg(color.alpha()));





//    QGraphicsBlurEffect *blr=new QGraphicsBlurEffect;
//    setGraphicsEffect(blr);
//QPixmap pix;
//pix.grabWidget(this);
//    QPainter *painter;
//    painter->set

} // setupUi


//void WidgetPopup::setWidget(QWidget *w)
//{

//hLayout->addWidget(w);

//}

WidgetPopup::~WidgetPopup()
{
   // delete ui;
}



void WidgetPopup::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    realign();
}

void WidgetPopup::open(QPoint pos, int s)
{

    m_pos = pos;

    pMap=s;

    realign();
    show();
    QTimer::singleShot(5,this,SLOT(resizeMe()));

}
void WidgetPopup::resizeMe()
{
//    resize(75,50);
    adjustSize();
    realign();
    int p1=pMap-this->pos().x()-22;
    if(p1>this->width()-56)
        p1=this->width()-56;
    if(p1<1)
        p1=1;

    widgetTop->move(p1,0);
    widgetBot->move(p1,0);
}

void WidgetPopup::realign()
{


    if(QX11Info::isCompositingManagerRunning()){
        widgetTopContener->setVisible(false);
        widgetBotContener->setVisible(false);
        mMargin=7;
    }else{
        widgetTopContener->setVisible(false);
        widgetBotContener->setVisible(false);
       mMargin=1;
    }

 gridLayoutPopup->setContentsMargins(mMargin,mMargin,mMargin,mMargin);
    QRect screen = QApplication::desktop()->availableGeometry(m_pos);
qDebug()<<"QApplication::desktop()->availableGeometry(m_pos);"<<QApplication::desktop()->availableGeometry(m_pos);
    QRect rect;

    rect.setSize(this->size());

    rect.moveTo(m_pos-QPoint((this->width()/2),(this->height()/2)));


    if (rect.right() > screen.right())
        rect.moveRight(screen.right());


    if (rect.bottom() > screen.bottom()){
        rect.moveBottom(screen.bottom()+mMargin);
        widgetTop->setVisible(false);
        widgetBot->setVisible(true);
    }

    if (rect.left() < screen.left())
        rect.moveLeft(screen.left());

    if (rect.top() < screen.top()){
        rect.moveTop(screen.top());
        widgetTop->setVisible(true);
        widgetBot->setVisible(false);
    }

    move(rect.topLeft());
}
