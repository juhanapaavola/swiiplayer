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

#ifndef WAITINGDIALOG_H
#define WAITINGDIALOG_H

#include <QDialog>
#include <QPoint>
#include <QGraphicsPixmapItem>

class QTimer;
class QPropertyAnimation;

class MyItem: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:

    MyItem(const QPixmap& pm):QGraphicsPixmapItem(pm)
    {
    }
};

namespace Ui {
    class WaitingDialog;
}

class WaitingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WaitingDialog(QWidget *parent = 0);
    virtual ~WaitingDialog();

public slots:
    void slotCloseDialog(int result);

private slots:
    void timeout();
    void animFinished();

private:
    Ui::WaitingDialog   *ui;
    QTimer*              m_timer;
    QPoint               m_startPoint;
    QPoint               m_endPoint;
    QPropertyAnimation*  m_anim;
    MyItem*              m_ellipse;
    int                  m_rot;
};

#endif // WAITINGDIALOG_H
