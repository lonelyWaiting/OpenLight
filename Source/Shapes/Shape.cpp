#include "PCH.h"
#include "IO/Log.h"
#include "Math/Normal.h"
#include "Math/Point3.h"
#include "Math/Ray.h"
#include "Math/Transform.h"
#include "Math/Vector3.h"
#include "Shape.h"

Shape::Shape( const Transform* _ObjectToWorld , const Transform* _WorldToObject )
{
	// 这里使用memcpy的原因是
	// 传入的Transform有可能是在某个函数中创建的，然后出了作用域之后就被销毁了
	// 如在某个Paser函数解析场景，然后创建了Transform对象
	// 然后传入Shape(&_ObjectToWorld , &_WorldToObject)
	// 但是出了作用域之后，该内存会被销毁，当然如果是new出来的，则不会
	// 但最好还是直接将数据拷贝过来
	// 然后在析构函数中释放一下
	ObjectToWorld = new Transform;
	memcpy( ObjectToWorld , _ObjectToWorld , sizeof( Transform ) );

	WorldToObject = new Transform;
	memcpy( WorldToObject , _WorldToObject , sizeof( Transform ) );
}

Shape::~Shape()
{
	SAFE_DELETE( ObjectToWorld );
	SAFE_DELETE( WorldToObject );
}

Bound3f Shape::WorldBound() const
{
	return ( *ObjectToWorld )( ObjectBound() );
}

bool Shape::Intersect( const Ray& ray , IntersectRecord* record ) const
{
	Assert( "Called Unimplemented Shape::Intersect()" );

	return false;
}

bool Shape::IntersectP( const Ray& ray ) const
{
	Assert( "Called Unimplemented Shape::IntersectP()" );

	return false;
}