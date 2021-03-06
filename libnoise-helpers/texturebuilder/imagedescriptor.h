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

#ifndef IMAGEDESCRIPTOR_H
#define IMAGEDESCRIPTOR_H

#include <QObject>

#include <QObject>
#include <QtCore>
#include <tuple>
#include <noiseutils.h>
#include <ssg_structures.h>
#include "libnoise-helpers_global.h"

class LIBNOISEHELPERSSHARED_EXPORT ImageDescriptor : public QObject
{
    Q_OBJECT

    QString _name = "image1";
public:
    QString& name() { return _name; }
    void setName(const QString& v) { _name = v; }
    ImageDescriptor();

    QString luaDeclaration() { return _name + "=Image.new()\n" + _name +"Bmp=WriterBMP.new()"; }
    QString luaInitialization();


    //from descriptor to actual object
    QSharedPointer<utils::Image> makeImage() {
        auto p = new utils::Image();
        QSharedPointer<utils::Image> sp; sp.reset(p);
        return sp;
    }
};

#endif // IMAGEDESCRIPTOR_H
