/*
############################################################################
#
# This file is part of Warp2010, (C) Massimiliano Lambertini - 2009
# Contact: m.lambertini@gmail.com
#
# GNU General Public License Usage
# This file may be used under the terms of the GNU
# General Public License version 3.0 as published by the Free Software
# Foundation and appearing in the file LICENSE.GPL included in the
# packaging of this file.  Please review the following information to
# ensure the GNU General Public License version 3.0 requirements will be
# met: http://www.gnu.org/copyleft/gpl.html.
#
############################################################################
*/


#include "planetgraphicsitem.h"
#include <math.h>
#include "helpers/pixmaphelper.h"
#include "helpers/planetselector.h"
#include "helpers/preferences.h"
#include "helpers/planetrenderer.h"

PlanetGraphicsItem::PlanetGraphicsItem(double x, double y, Planet *planet, double scaleFactor)
{
    _planet = 0;
    _selector = new PlanetSelector();
    this->setFlag(QGraphicsItem::ItemIsSelectable,true);
    _planet = planet;
    if (!_planet.isNull()) {
        double dSize = _planet->diameter();
        double lSize = 0.0;
        if (dSize <= 12000.0)
            lSize = dSize / 100.0;
        else {
            lSize = 120 * (1.0+ log( ( dSize/ 12000.0 )));
        }
        if (lSize < 10.0)
            lSize = 10.0;
        _boundingRect = QRectF ( x, y , lSize, lSize);

        QFont font;
        if (_planet->isSatellite())
            Preferences::prefsPtr()->fontSmall();
        else
            Preferences::prefsPtr()->fontBody();
        QFontMetricsF fm(font);

        QString  sPlanetDesc =
                _planet->name()+"\n"+
                _planet->getPlanetTypeDesc() + "\n"+
                QString("%1 AU").arg(_planet->orbit().distance());

        QRectF r1;
        QRectF r2 = fm.boundingRect(r1,Qt::AlignHCenter | Qt::AlignVCenter,sPlanetDesc);

        _textRect = r2;


        _planetRect = _boundingRect;
        qreal tw = _textRect.width();
        qreal th = _textRect.height();
        //_boundingRect = _planetRect.unite(textRect);

        _textRect.setTopLeft(_boundingRect.bottomLeft());
        _textRect.setWidth(tw);
        _textRect.setHeight(th);

        _boundingRect = _planetRect.united(_textRect);


        if (_planetRect.width() < _textRect.width()) {
            qreal dx = (_textRect.width()- _planetRect.width() ) / 2.0;
            _planetRect.translate(dx,0);
        }
        else {
            qreal dx = (_planetRect.width()- _textRect.width() ) / 2.0;
            _textRect.translate(dx,0);

        }



        _boundingRect = _boundingRect.united(_textRect);



    }

}

PlanetGraphicsItem::~PlanetGraphicsItem() {
    delete _selector;
    //qDebug() << " ~PlanetGraphicsItem invoked ";
}

QRectF PlanetGraphicsItem::boundingRect() const {
    return _boundingRect;
}

QRectF PlanetGraphicsItem::planetRect() const {
    return _planetRect;
}

QRectF PlanetGraphicsItem::textRect() const {
    return _textRect;
}

void PlanetGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if (!_planet.isNull()) {


        _boundingRect = planetRect();
        qreal x = boundingRect().x();
        qreal y = boundingRect().y();
        qreal h = boundingRect().height();
        qreal w = boundingRect().width();

        //qDebug() << "Bounding Rect: " << _boundingRect  << "," << _planetRect << "," << _textRect;

        QString unit = _planet->isSatellite() ? "Km" : "AU";
        QString planetDesc = "none";
        Q_ASSERT(_planet != NULL );
        Q_ASSERT(!_planet.isNull() );

        if (!_planet.isNull() && _planet->planetType() != NSPlanet::ptNone) {
            //qDebug() << _planet;
            //qDebug() << _planet->name();
            planetDesc =
                    _planet->name()+"\n"+
                    _planet->getPlanetTypeDesc() + "\n"+
                    QString("%1 %2").arg(_planet->orbit().distance()).arg(unit);
            Planet *p = _planet;
            PlanetRenderer::current()->setPlanet(p);
            this->setToolTip(PlanetRenderer::current()->description());
        }
        QFont font;
        if (_planet->isSatellite())
            Preferences::prefsPtr()->fontSmall();
        else
            Preferences::prefsPtr()->fontBody();
        QFontMetrics fm(font);
        QRectF txtRect = fm.boundingRect(planetDesc);

        int planetType = (int)_planet->planetType();
        painter->setRenderHint(QPainter::Antialiasing);

        int nType = (int)(_planet->planetType());
        QPixmap* icon = PixmapHelper::instancePtr()->planetaryPics(nType);
        if (icon != 0) {
            QImage result = icon->toImage().scaled((int)(w),(int)(h),
                                       Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
            painter->drawImage(QPointF(x,y),result);
            if (this->isSelected()) {
                QPen pen(QColor(255,255,0,192));
                pen.setWidthF(5.0);
                QBrush brush;
                painter->setPen(pen);
                painter->setBrush(brush);
                QRectF rectEll = boundingRect().adjusted(+2.5,+2.5,-2.5,-2.5);
                painter->drawEllipse(rectEll);
            }
            QBrush black(QColor(0,0,0));
            QPen  pen(QColor(0,0,0));
            painter->setBrush(black);
            painter->setPen(pen);
            //qDebug() << planetDesc;
            painter->drawText(textRect(),Qt::AlignHCenter,planetDesc);
        }

    }
}

void PlanetGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    this->setSelected(true);
    _selector->emitPlanetSelected(_planet);
}

