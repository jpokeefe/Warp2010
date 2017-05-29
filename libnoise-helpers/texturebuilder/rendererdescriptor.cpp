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

#include "rendererdescriptor.h"

RendererDescriptor::RendererDescriptor()
{

}

QSharedPointer<utils::RendererImage> RendererDescriptor::makeRenderer() {
    utils::RendererImage* p = new utils::RendererImage();

    p->SetBackgroundColor(utils::Color(0,0,0,0)); //transparency, quoi
    p->EnableLight(this->_enabledLight);
    p->SetLightBrightness(this->lightBrightness());
    p->SetLightContrast(this->lightContrast());

    //and now, gradient info
    p->ClearGradient();
    GradientInfo gi;
    foreach (gi,_gradientInfo) {
        p->AddGradientPoint(std::get<0>(gi), utils::Color(std::get<1>(gi),std::get<2>(gi),std::get<3>(gi),std::get<4>(gi)));
    }

    QSharedPointer<utils::RendererImage> sp; sp.reset(p);
    return sp;
}

RendererDescriptor& RendererDescriptor::connectImagesAndMap() {
    utils::RendererImage* p = _renderers[this->name()].data();
    if (_backgroundImage != "") {
        auto pImg = _images[_backgroundImage].data(); //pointer to utils::Image
        p->SetBackgroundImage(*pImg);                 //reference to stored data
    }
    if (_destImage != "") {
        auto pImg = _images[_destImage].data(); //pointer to utils::Image
        p->SetDestImage(*pImg);                 //reference to stored data
    }
    auto theMap = _noiseMaps[this->_noiseMap].data();
    qDebug() << "x: " << theMap->GetWidth() << ", y: " <<  theMap->GetHeight();
    p->SetSourceNoiseMap(*theMap);
    return *this;
}

void RendererDescriptor::toJson(QJsonObject& json) {

    json["name"] = _name;
    json["noiseMap"] = _noiseMap;
    json["enabledLight"] = _enabledLight;
    json["lightContrast"] = _lightContrast;
    json["lightBrightness"] = _lightBrightness;
    json["backgroundImage"] = _backgroundImage;
    json["destImage"] = _destImage;
    json["randomGradient"] = _randomGradient;

    QJsonArray giItems;
    GradientInfo gi;
    foreach (gi, _gradientInfo) {
        QJsonArray a;
        a.append(std::get<0>(gi));
        a.append(std::get<1>(gi));
        a.append(std::get<2>(gi));
        a.append(std::get<3>(gi));
        a.append(std::get<4>(gi));
        giItems.append(a);
    }
    json["gradientInfo"] = giItems;
}

void RendererDescriptor::fromJson(const QJsonObject& json) {
    if (!json["randomGradient"].isNull() && !json["randomGradient"].isUndefined())
        _randomGradient = json["randomGradient"].toBool();
    if (!json["randomFactor"].isNull() && !json["randomFactor"].isUndefined() && json["randomFactor"].isArray() )
    {
        auto jArray = json["randomFactor"].toArray();
        //if (jArray.count() != 3)
        //    throw noise::ExceptionInvalidParam;
        //hue, sat, v;
        _rndHue = jArray[0].toInt() % 255;
        _rndSaturation = jArray[1].toInt() % 255;
        _rndValue = jArray[2].toInt() % 255;

    }

    if (!json["destImage"].isNull() && !json["destImage"].isUndefined())
        _destImage = json["destImage"].toString();
    if (!json["backgroundImage"].isNull() && !json["backgroundImage"].isUndefined())
        _backgroundImage = json["backgroundImage"].toString();
    if (!json["name"].isNull() && !json["name"].isUndefined())
        _name = json["name"].toString();
    if (!json["noiseMap"].isNull() && !json["noiseMap"].isUndefined())
        _noiseMap = json["noiseMap"].toString();
    if (!json["enabledLight"].isNull() && !json["enabledLight"].isUndefined())
        _enabledLight = json["enabledLight"].toBool();
    if (!json["lightContrast"].isNull() && !json["lightContrast"].isUndefined())
        _lightContrast = json["lightContrast"].toDouble();
    if (!json["lightBrightness"].isNull() && !json["lightBrightness"].isUndefined())
        _lightBrightness = json["lightBrightness"].toDouble();
    if (!json["gradientInfo"].isNull() && !json["gradientInfo"].isUndefined() && !_randomGradient) {
        QJsonArray gi = json["gradientInfo"].toArray();
        _gradientInfo.clear();
        bool mustApplyRandomFactor = this->applyRandomFactor();
        for (auto h = 0; h < gi.size(); h++) {
            QJsonArray i = gi[h].toArray();
            if (mustApplyRandomFactor) {
                utils::Color c(i[1].toInt(),i[2].toInt(),i[3].toInt(),i[4].toInt()) ;
                c = c.change(_rndHue, _rndSaturation, _rndValue);
                auto grad = GradientInfo(i[0].toDouble(),
                                        static_cast<int>(c.red),
                                         static_cast<int>(c.green),
                                         static_cast<int>(c.blue),
                                         static_cast<int>(c.alpha));
                _gradientInfo.append(grad);
            } else {
                auto grad = GradientInfo(i[0].toDouble(),
                    i[1].toInt(),i[2].toInt(),i[3].toInt(),i[4].toInt());
                _gradientInfo.append(grad);
            }
        }
    }
    else {
        double dRes = -1.0;
        auto startingColor = utils::Color::RandomColor();
        startingColor.alpha = 255;
        bool direction = SSGX::d10() % 2 == 0;
        while (dRes < 1.0) {
            auto grad = GradientInfo(dRes, static_cast<int>(startingColor.red),
                                     static_cast<int>(startingColor.green),
                                     static_cast<int>(startingColor.blue),
                                     static_cast<int>(startingColor.alpha));
            _gradientInfo.append(grad);
            qDebug() << std::get<0>(grad) << "," << std::get<1>(grad) << "," << std::get<2>(grad) << "," << std::get<3>(grad);
            dRes +=  0.1+SSGX::floatRand()*0.25;
                startingColor = startingColor.change(10,20,30);
            if (SSGX::d6() == 4)
                direction = !direction;
        }
        auto grad = GradientInfo(1.0, static_cast<int>(startingColor.red),
                                 static_cast<int>(startingColor.green),
                                 static_cast<int>(startingColor.blue),
                                 static_cast<int>(startingColor.alpha));
        _gradientInfo.append(grad);

    }

}