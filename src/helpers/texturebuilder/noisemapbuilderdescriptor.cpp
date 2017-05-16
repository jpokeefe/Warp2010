#include "noisemapbuilderdescriptor.h"

using namespace std;
using namespace noise;

NoiseMapBuilderDescriptor::NoiseMapBuilderDescriptor(QObject *parent) : QObject(parent)
{

}

QSharedPointer<utils::NoiseMapBuilder> NoiseMapBuilderDescriptor::makeBuilder() {
    switch (_builderType) {
        case NoiseMapBuilderType::CYLINDER:
            return makeCylinderBuilder();
        break;
    case NoiseMapBuilderType::PLANE:
        return makePlaneBuilder();
    break;
    case NoiseMapBuilderType::SPHERE:
        return makeSphereBuilder();
    break;
    default:
        return makeSphereBuilder();
    break;
    }
}

QSharedPointer<utils::NoiseMapBuilder> NoiseMapBuilderDescriptor::makeCylinderBuilder() {
    utils::NoiseMapBuilderCylinder *p = new utils::NoiseMapBuilderCylinder();
    p->SetBounds( std::get<0>(_bounds)
                 ,std::get<1>(_bounds)
                 ,std::get<2>(_bounds)
                 ,std::get<3>(_bounds));
    p->SetDestSize(std::get<0>(_size), std::get<1>(_size));
    p->SetSourceModule( *this->_modules[this->_src1].data());
    p->SetDestNoiseMap( * this->_maps[this->_dest].data());
    QSharedPointer<utils::NoiseMapBuilder> sp; sp.reset(p);
    return sp;
}

QSharedPointer<utils::NoiseMapBuilder> NoiseMapBuilderDescriptor::makePlaneBuilder() {
    utils::NoiseMapBuilderPlane *p = new utils::NoiseMapBuilderPlane();
    p->SetBounds( std::get<0>(_bounds)
                 ,std::get<1>(_bounds)
                 ,std::get<2>(_bounds)
                 ,std::get<3>(_bounds));
    p->SetDestSize(std::get<0>(_size), std::get<1>(_size));
    p->SetSourceModule( *this->_modules[this->_src1].data());
    p->SetDestNoiseMap( * this->_maps[this->_dest].data());

    QSharedPointer<utils::NoiseMapBuilder> sp; sp.reset(p);
    return sp;

}

QSharedPointer<utils::NoiseMapBuilder> NoiseMapBuilderDescriptor::makeSphereBuilder() {
    utils::NoiseMapBuilderSphere *p = new utils::NoiseMapBuilderSphere();
    p->SetBounds( std::get<0>(_bounds)
                 ,std::get<1>(_bounds)
                 ,std::get<2>(_bounds)
                 ,std::get<3>(_bounds));
    p->SetDestSize(std::get<0>(_size), std::get<1>(_size));
    p->SetSourceModule( *this->_modules[this->_src1].data());
    p->SetDestNoiseMap( * this->_maps[this->_dest].data());
    QSharedPointer<utils::NoiseMapBuilder> sp; sp.reset(p);
    return sp;

}