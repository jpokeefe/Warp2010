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

#ifndef SCENEIMAGEEXPORTER_H
#define SCENEIMAGEEXPORTER_H

#include <QtGui>
#include <QGraphicsScene>
#include <QPrinter>
#include "helpers/sectorrectholder.h"

namespace SIEOptions {
    const unsigned int SIE_USE_OVERLAY_BORDER = 1;
    const unsigned int SIE_SCALE_TO_SCENE_HEIGHT = 2;
    const unsigned int SIE_SCALE_TO_SCENE_WIDTH = 4;
    const unsigned int SIE_DRAW_MINIATURE_MAP = 8;
    const unsigned int SIE_PRINT_ON_ONE_PAGE = 16;

    const unsigned int SIE_DEFAULT = SIE_USE_OVERLAY_BORDER | SIE_DRAW_MINIATURE_MAP;
    
    inline bool ScaleToSceneHeight (int i) { return  (i & SIE_SCALE_TO_SCENE_HEIGHT); }
    inline bool ScaleToSceneWidth (int i) { return  (i & SIE_SCALE_TO_SCENE_WIDTH); }
    inline bool DrawMiniatureMap (int i) { return (i & SIE_DRAW_MINIATURE_MAP); }
    inline bool UseOverlayBorder (int i) { return (i & SIE_USE_OVERLAY_BORDER); }
}

class SceneImageExporter
{
private:
    int _verticalDivisions;
    int _horizontalDivisions;
    bool _useShadedOverlayArea;
    bool _canDrawMiniatureMap;
    double _shadedOverlayAreaSize;


public:
    SceneImageExporter() :
            _verticalDivisions(1),
            _horizontalDivisions(1),
            _useShadedOverlayArea(false),
            _canDrawMiniatureMap(false),
            _shadedOverlayAreaSize(0.10)
    {}


    //static members, to be invoked directly
    static void ExportToSVG (QGraphicsScene *scene, QString filename);
    static void ExportToPNG (QGraphicsScene *scene, QString filename);
    static void printScene(QGraphicsScene *scene, QPrinter& printer,
                           QString fileName = "", QString documentName = "",
                           int printingFlags = SIEOptions::SIE_DEFAULT);


    //public instance members;
    inline void setVerticalDivisions(int v) { _verticalDivisions = v; }
    inline void setHorizontalDivisions(int v) { _horizontalDivisions = v; }
    inline void setUseShadedOverlayArea(bool v) { _useShadedOverlayArea = v; }
    inline void setCanDrawMiniatureMap(bool v) { _canDrawMiniatureMap = v; }
    inline void setShadedOverlayAreaSize(double v) { _shadedOverlayAreaSize = v; }

    inline int verticalDivisions() { return _verticalDivisions; }
    inline int horizontalDivisions() { return _horizontalDivisions; }
    inline bool useShadedOverlayArea() { return _useShadedOverlayArea; }
    inline bool canDrawMiniatureMap() { return _canDrawMiniatureMap; }
    inline double shadedOverlayAreaSize() { return _shadedOverlayAreaSize; }

    void GraphicsSceneToPrinter(QGraphicsScene *scene, QPrinter& printer,
                           QString fileName = "", QString documentName = "",
                           bool bSizeToOneSheet = false);
    void PageGraphicsSceneToPrinter(QGraphicsScene *scene, QPrinter& printer,
                                    QList<SectorRectHolder>& pageRegions,QString fileName = "",
                                    QString documentName = "");
};

#endif // SCENEIMAGEEXPORTER_H
