#ifndef QUARK_MESHER_HH
#define QUARK_MESHER_HH


#include <iostream>
#include <string>
#include <list>
#include <engine/Vector.hh>


struct MesherFace
{
	Vector3f vertex;
	Vector3f normal;
	Vector2f uv;
};


struct MesherObject
{
    std::string name;
    std::list<MesherFace> faces;
};


struct MesherModel
{
    std::list<MesherObject> objects;

    MesherModel();

    MesherModel(
        std::istream &in );

    MesherModel(
        const std::string &fileName );

    MesherModel &operator+=(
        MesherObject &object );

    void save(
        std::ostream &out );

    void save(
        const std::string &fileName );

    void load(
        std::istream &in );
};



#endif // QUARK_MESHER_HH