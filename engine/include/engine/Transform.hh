#ifndef ENGINE_TRANSFORM_HH
#define ENGINE_TRANSFORM_HH


#include <engine/Vector3.hh>
#include <engine/Matrix4.hh>


class Transform
{
    public:
        #ifdef NAN
        constexpr static const float KEEP = NAN;
        #else
        constexpr static const float KEEP = -1;
        #endif

        Transform();

        void translate( const Vector3f &value );

        void moveTo( const Vector3f &position );

        const Matrix4f &getMatrix();

        const Matrix4f &getMatrix() const;

        void rotate( float x, float y, float z );

        void rotate( const Vector3f &angles );

        void scale( float x, float y, float z );

        void scale( const Vector3f &angles );

        void reset();

        void update();

    private:
        Matrix4f matrix_;
        Vector3f position_;
        Vector3f angles_;
        Vector3f scales_;
        bool changed_;


};



#endif // ENGINE_TRANSFORM_HH