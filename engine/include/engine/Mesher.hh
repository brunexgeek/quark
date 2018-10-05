#ifndef QUARK_MESHER_HH
#define QUARK_MESHER_HH


#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <engine/Vector.hh>


#define MESHER_MAX_OBJ_NAME      (32 - 1)


/**
 * Triangular faces which each vertex having:
 *
 * 12 bytes (3 x float)  for position
 *  4 bytes (3 x float)  for normal or color
 *  8 bytes (2 x float)  for UV
 */
#define MESHER_FORMAT_VNU_24     ((uint32_t) 1)


#pragma pack(push, 4)

struct MesherFaceVNU24
{
	Vector3f vertex;
	Vector3f normal;
	Vector2f uv;
};

#pragma pack(pop)


struct MesherObject
{
    std::string name;
    std::shared_ptr<std::vector<Vector3f>> vertices;
    std::shared_ptr<std::vector<Vector3f>> normals;
    std::shared_ptr<std::vector<Vector2f>> uvs;
    std::shared_ptr<std::vector<Vector3u>> faces;

    MesherObject() : vertices(new std::vector<Vector3f>), normals(new std::vector<Vector3f>),
        uvs(new std::vector<Vector2f>), faces(new std::vector<Vector3u>)
    {
    };

    MesherObject( const MesherObject &obj ) : vertices(new std::vector<Vector3f>), normals(new std::vector<Vector3f>),
        uvs(new std::vector<Vector2f>), faces(new std::vector<Vector3u>)
    {
        name     = obj.name;
        vertices = obj.vertices;
        normals  = obj.normals;
        uvs      = obj.uvs;
        faces    = obj.faces;
    }

    MesherObject( MesherObject &&obj )
    {
        name     = obj.name;
        vertices = obj.vertices;
        normals  = obj.normals;
        uvs      = obj.uvs;
        faces    = obj.faces;
    };
};


struct MesherModel
{
    std::vector<MesherObject> objects;

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