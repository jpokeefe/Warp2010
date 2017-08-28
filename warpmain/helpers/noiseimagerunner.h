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

#ifndef NOISEIMAGERUNNER_H
#define NOISEIMAGERUNNER_H

#include <QRunnable>
#include <noiseimageutils.h>
#include <ssg_structures.h>
#include <QString>
#include <QThread>
#include <planetmaps/maps.h>
#include <QSharedPointer>
#include <memory>
#include <texturebuilder/texturebuilder.h>
#include <ringtexturebuilder.h>

namespace RT {
enum RType {
    GG2,
    GG ,
    Cloudy,
    Earthlike,
    Earthlike2,
    Earthlike3 ,
    Glacier,
    Pregarden,
    Postgarden,
    Clouds,
    FunkyClouds,
    Desert,
    DesertG,
    ComplexDesert,
    ComplexDesert2,
    Jade,
    Jade2,
    Granite,
    Ice,
    UseBuilder,
    Ring
};
}

using namespace RT;

class NoiseImageRunner : public QObject
{
    Q_OBJECT
    int    _seed;
    int    _octave;
    double _persistence;
    double _lacunarity;
    double _frequency;

    double _seaRatio = 0.0;
    double _iceRatio = 0.125;


    RType _runType = Earthlike;
    QString _filename = "";
    QString _textureFile = "";
    NoiseImageUtils imgUtils;
    QString _planetName = "";
    QString _planetType = "";
public:    
    void setSeed (int v) {_seed = v; }
    int  seed() { return _seed; }
    void setOctave (int v) {_octave = v; }
    int  octave() { return _octave; }
    void setPersistence(double v)  { _persistence = v; }
    double persistence() { return _persistence; }
    void setLacunarity(double v) { _lacunarity = v; }
    double lacunarity() { return _lacunarity; }
    void setFrequency(double v) {_frequency = v; }
    double frequency() { return _frequency; }
    void setFilename(const QString& str) { _filename = str; }
    const QString& filename() { return _filename; }
    void setTextureFile(const QString& str) { _textureFile = str; }
    const QString& textureFile() { return _textureFile; }

    void setRunType(RType str) { _runType = str; }
    const RType runType() { return _runType; }
    double iceRatio() { return _iceRatio; }
    void setIceRatio (double v) { _iceRatio = v; }
    double seaRatio() { return _seaRatio; }
    void setSeaRatio (double v) { _seaRatio = v; }



    NoiseImageRunner();

    NoiseImageRunner(RType runType, const QString& filename, int seed) : _runType(runType), _filename(filename), _seed(seed) {}
    NoiseImageRunner(RType runType, const QString& filename, int seed, int octave) : _runType(runType),_filename(filename),_seed(seed), _octave(octave) {}
    NoiseImageRunner(RType runType, const QString& filename, int seed, int octave, double pers) :
        _runType(runType),_filename(filename),
        _seed(seed), _octave(octave),
        _persistence(pers){}
    NoiseImageRunner(RType runType, const QString& filename, int seed, int octave, double pers, double lac) :
        _runType(runType),_filename(filename),
        _seed(seed), _octave(octave),
        _persistence(pers),_lacunarity(lac){}
    NoiseImageRunner(RType runType, const QString& filename, int seed, int octave, double pers, double lac, double freq) :
        _runType(runType),_filename(filename),
        _seed(seed), _octave(octave),
        _persistence(pers),_lacunarity(lac), _frequency(freq){}

    static QSharedPointer<NoiseImageRunner> UseTextureBuilder(const QString& textureFile, const QString& imageFile);
    static QSharedPointer<NoiseImageRunner> UseRingBuilder(const QString& imageFile);

    void run()  ;
    void setPlanetNameAndType (const QString& planetName, const QString& planetType) { _planetName = planetName; _planetType = planetType; }

signals:
    void imageSaved(QString f);

};

#endif // NOISEIMAGERUNNER_H
