/*
############################################################################
#
# This file is part of Warp2010, (C) Massimiliano Lambertini - 2009
# Contact: m.lambertini@gmail.com
#
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA#
############################################################################
*/

#ifndef VIEWINGDIALOG_H
#define VIEWINGDIALOG_H

#include <QDialog>
#include <QFrame>
#include <QLabel>
#include <QScrollArea>

namespace Ui {
class ViewingDialog;
}

class ViewingDialog : public QDialog
{
    Q_OBJECT

    QFrame *frame;
    Ui::ViewingDialog *ui;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
public:
    explicit ViewingDialog(QWidget *parent = 0);
    ~ViewingDialog();

    void loadImage(QString& filename);
};

#endif // VIEWINGDIALOG_H
