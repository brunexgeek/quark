#ifndef ENGINE_TRANSFORM_HH
#define ENGINE_TRANSFORM_HH


#include <engine/Vector3.hh>
#include <engine/Matrix4.hh>


class Transform
{
    public:
        Transform();

        void translate( const Vector3f &value );

        void moveTo( const Vector3f &position );

        const Matrix4f &getMatrix();

        const Matrix4f &getMatrix() const;

        void rotate( float x, float Y, float z );

        void rotate( const Vector3f &angles );

        void reset();

        void update();

    private:
        Matrix4f matrix_;
        Vector3f position_;
        Vector3f angles_;
        bool changed_;


};



#endif // ENGINE_TRANSFORM_HH