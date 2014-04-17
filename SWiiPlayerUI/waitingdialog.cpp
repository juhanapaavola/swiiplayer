/*
 * SWiiPlayerUI - Connecting WiiMote to Symbian phone
 * Copyright (C) 2010 Juhana Paavola <juhana.paavola@gmail.com>
 *
 * This file is part of SWiiPlayerUI.
 *
 * SWiiPlayerUI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SWiiPlayerUI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with SWiiPlayerUI.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "waitingdialog.h"
#include "ui_waitingdialog.h"
#include <QPixmap>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPropertyAnimation>

WaitingDialog::WaitingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitingDialog)
{
    ui->setupUi(this);

    m_rot = 0;

    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);

    QRect rect(0,0,50,50);

    m_startPoint = QPoint(0,ui->graphicsView->rect().size().height()/2);
    m_endPoint = QPoint(ui->graphicsView->rect().size().width(),ui->graphicsView->rect().size().height()/2);

    QRadialGradient rg = QRadialGradient(10,10,45.0);

    QColor mcol(Qt::blue);
    mcol.setAlpha(255);
    rg.setColorAt(0.0,Qt::white);
    mcol.setBlue(mcol.blue()-15);
    rg.setColorAt(0.5,mcol);
    mcol.setBlue(mcol.blue()+15);
    rg.setColorAt(1.0,mcol);

    QPen pen(mcol);
    QPixmap pm(rect.size()+QSize(1,1));
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setBackgroundMode(Qt::TransparentMode);
    p.setBrush(rg);
    p.setPen(pen);
    p.drawEllipse(rect);

    m_ellipse = new MyItem(pm);
    m_ellipse->setPos(m_startPoint);
    ui->graphicsView->scene()->addItem(m_ellipse);

    m_anim = new QPropertyAnimation(m_ellipse,"pos");
    connect(m_anim,SIGNAL(finished()),this,SLOT(animFinished()));
    m_anim->setEasingCurve(QEasingCurve::OutBounce);
    m_anim->setStartValue(m_startPoint);
    m_anim->setEndValue(m_endPoint);
    m_anim->setDuration(2000);
    m_anim->setLoopCount(1);
    m_anim->start();

    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeout()));
    m_timer->setInterval(100);

#if defined(Q_OS_SYMBIAN)
    this->showMaximized();
#else
    this->show();
#endif
    m_timer->start();
}

WaitingDialog::~WaitingDialog()
{
    delete ui;    
}

void WaitingDialog::slotCloseDialog(int result)
{
    if(result<0)
        this->reject();
    else
        this->accept();
}

void WaitingDialog::animFinished()
{
    QVariant v(m_anim->startValue());
    QPoint p = v.toPointF().toPoint();
    if(p == m_startPoint){
        m_anim->setStartValue(m_endPoint);
        m_anim->setEndValue(m_startPoint);
    }else{
        m_anim->setStartValue(m_startPoint);
        m_anim->setEndValue(m_endPoint);
    }
    m_anim->start();
}

void WaitingDialog::timeout()
{
    int x = m_ellipse->pixmap().size().width()/2;
    int y= m_ellipse->pixmap().size().height()/2;

    m_ellipse->translate(x,y);
    m_ellipse->rotate(m_rot);
    m_ellipse->translate(-x,-y);
    m_rot++;
}
