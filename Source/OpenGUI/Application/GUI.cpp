#include "GUI.h"
#include "imgui.h"
#include "OpenLight.h"

void ShowPropertyWindow( Scene* scene , Renderer* renderer , int index , bool& bShow )
{
	Shape* shape = ( scene->GetPrimitive( index ) ).GetPrimitiveObject( 0 );

	ImGui::SetNextWindowSize( ImVec2( 200 , 100 ) , ImGuiSetCond_FirstUseEver );
	ImGui::Begin( scene->GetPrimitive( index ).GetName() , &bShow );

	// -------------------------------------------------------------------------------------
	{
		Point3f Pos = shape->GetPosition();

		float input[3] = { Pos[0] , Pos[1] , Pos[2] };

		if( ImGui::InputFloat3( "position" , &input[0] ) )
		{
			shape->SetPosition( &input[0] );

			renderer->GetAccelerator()->Reset();
		}
	}
	
	// -----------------------------------------------------------------------------------
	{
		Spectrum SurfaceColor = shape->GetSurfaceColor();

		float input[3] = { SurfaceColor[0] , SurfaceColor[1] , SurfaceColor[2] };

		if( ImGui::InputFloat3( "SurfaceColor" , input ) )
		{
			shape->SetSurfaceColor( input );
		}
	}

	//------------------------------------------------------------------------------------
	{
		if( !strcmp( shape->GetName() , "Sphere" ) )
		{
			float radius = dynamic_cast< Sphere* >( shape )->GetRadius();

			if( ImGui::InputFloat( "Radius" , &radius ) )
			{
				dynamic_cast< Sphere* >( shape )->SetRadius( radius );
			}
		}
	}

	// --------------------------------------------------------------------------------
	{
		Material* pMaterial = scene->GetPrimitive( index ).GetMaterial();

		ImGui::Text( pMaterial->GetName() );

		if( !strcmp( pMaterial->GetName() , "PureReflectionMaterial" ) )
		{
			Spectrum R = dynamic_cast< PureReflectionMaterial* >( pMaterial )->GetReflection();

			float r[3] = { R[0] , R[1] , R[2] };

			if( ImGui::InputFloat3( "R" , r ) )
			{
				dynamic_cast< PureReflectionMaterial* >( pMaterial )->SetReflection( r );
			}
		}
		else if( !strcmp( pMaterial->GetName() , "GlassMaterial" ) )
		{
			GlassMaterial *pGlassMaterial = dynamic_cast< GlassMaterial* >( pMaterial );

			Spectrum R = pGlassMaterial->GetReflection();

			float r[3] = { R[0] , R[1] , R[2] };

			if( ImGui::InputFloat3( "R" , r ) )
			{
				pGlassMaterial->SetReflection( r );
			}

			Spectrum T = pGlassMaterial->GetTransmission();

			float t[3] = { T[0] , T[1] , T[2] };

			if( ImGui::InputFloat3( "T" , t ) )
			{
				pGlassMaterial->SetTransmission( t );
			}

			float ior = pGlassMaterial->GetIOR();

			if( ImGui::InputFloat( "IOR" , &ior ) )
			{
				pGlassMaterial->SetIOR( ior );
			}
		}
		else if( !strcmp( pMaterial->GetName() , "DiffuseMaterial" ) )
		{
			DiffuseMaterial* pDiffuseMaterial = dynamic_cast< DiffuseMaterial* >( pMaterial );

			Spectrum R = pDiffuseMaterial->GetReflection();

			float r[3] = { R[0] , R[1] , R[2] };

			if( ImGui::InputFloat3( "R" , r ) )
			{
				pDiffuseMaterial->SetReflection( r );
			}
		}
	}
	ImGui::End();
}