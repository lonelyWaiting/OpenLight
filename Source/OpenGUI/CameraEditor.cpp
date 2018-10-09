#include "CameraEditor.h"
#include "imgui.h"
#include "Camera/Camera.h"

void CameraEditor()
{
	Vector3f Pos = Vector3f( 0 );
	if( ImGui::SliderFloat( "Pos" , &Pos[0] , 0.0f , 100.0f ) )
	{

	}


}