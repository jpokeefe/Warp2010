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

#ifndef STARLIST_H
#define STARLIST_H

#include <QObject>
#include <QtCore>
#include "star.h"
#include "helpers/parsecstarlisthelper.h"

#include "parsecstar.h"

typedef QVector <QSharedPointer<Star>> QStarList;
typedef QList<int> QIdxStarList;

bool starLessThan(QSharedPointer<Star> st1, QSharedPointer<Star> st2);
bool starGreaterThan(QSharedPointer<Star> tr1, QSharedPointer<Star> tr2);


class StarList //: public QObject
{
    //Q_OBJECT

private:
    QString _listName;
    double  _jumpDistance;

    static StarList _starList;

    StarList (const StarList & );
    StarList& operator= (const StarList&);
    StarList();
    virtual ~StarList();
    QVector<ParsecStar> _parsecStarList;
    QVector<QSharedPointer<Star>> _stars;
    QStarList _qStarList;
    QIdxStarList _reachableStars;
    QIdxStarList _inTradeRoute;

public:

    //StarList();
    //StarList(const StarList &that);
    //virtual ~StarList();
    //TODO: implent the copy constructor, fucken idiom!

    //StarList(const QString& filename);

    static StarList& StarListRef() { return StarList::_starList; }
    static StarList* StarListPtr() { return &StarList::_starList; }

    void createRandomMap(double radius, double density, double dx = 0.0, double dy=0.0, double dz=0.0);
    void loadMap(const QString& filename) /*throw(std::exception)*/;
    void saveMap(const QString& filename);
    void calculateNeighbors(double distance);
    void buildMatrix(int idx = 0, bool isStartingMode = true);
    void rebuildMatrix(int idx);

    double calculatePathDistance(int idx);
    double calculatePathDistance(QVector<int> &);

    inline QString& listName() { return _listName; }
    inline void setListName(QString& name) { _listName = name; }
    inline double jumpDistance() { return _jumpDistance; }
    inline void setJumpDistance(double v) { _jumpDistance = v; }

    inline QVector<QSharedPointer<Star>>& stars() { return _stars;}
    inline int count() { return _stars.count();}
    QVector<ParsecStar>& prepareParsecStarList(SceneMediatorDrawMode::DrawMode mode = SceneMediatorDrawMode::XY);
    QStarList& qStarList() { return _qStarList; }

    QStarList& getQStarList(bool bGreaterThan = true);
    QIdxStarList& reachableStars() { return _reachableStars; }
    QIdxStarList& inTradeRoute() { return _inTradeRoute; }

    void buildReachableStars();
    bool isReachable(int idx) { return _reachableStars.contains(idx);  }

    void loadMatrix (QString filename);
    void toJson (QJsonObject& o);
    void fromJson(const QJsonObject& o);
    void saveToJson (QString filename);
    void loadFromJson (QString filename);
    void saveMatrix (QString filename);
    void serializeStarList (QDataStream& out);
    void deserializeStarList (QDataStream& in);
};


#endif // STARLIST_H
