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

		const char* MaterialName = pMaterial->GetName();

		ImGui::Text( MaterialName );

		ShowMaterialProperty( pMaterial , MaterialName );
	}
	
	ImGui::End();
}

void ShowMaterialProperty( Material* pMaterial , const char* MaterialTypeName )
{
	if( !strcmp( MaterialTypeName , "PureReflectionMaterial" ) )
	{
		PureReflectionMaterial* pReflectionMaterial = dynamic_cast< PureReflectionMaterial* >( pMaterial );

		Texture* pKr = pReflectionMaterial->Getkr();

		if( !strcmp( pKr->GetName() , "ConstantTexture" ) )
		{
			ConstantTexture* pConstantTexture = dynamic_cast< ConstantTexture* >( pKr );

			ShowConstantTextureProperty( pConstantTexture , "Kr" );
		}
		else if (!strcmp(pKr->GetName(), "ImageTexture"))
		{
			ImageTexture* pImageTexture = dynamic_cast<ImageTexture*>(pKr);

			ShowImageTextureProperty(pImageTexture, "Kr");
		}
	}
	else if( !strcmp( MaterialTypeName , "DiffuseMaterial" ) )
	{
		DiffuseMaterial* pDiffuseMaterial = dynamic_cast< DiffuseMaterial* >( pMaterial );

		Texture* pKd = pDiffuseMaterial->GetKd();

		if( !strcmp( pKd->GetName() , "ConstantTexture" ) )
		{
			ConstantTexture* pConstantTexture = dynamic_cast< ConstantTexture* >( pKd );

			ShowConstantTextureProperty( pConstantTexture , "kd" );
		}
		else if( !strcmp( pKd->GetName() , "CheckboardTexture" ) )
		{
			CheckboardTexture* pCheckboardTexture = dynamic_cast< CheckboardTexture* >( pKd );

			ShowCheckboardTextureProperty( pCheckboardTexture , "color1" , "color2" );
		}
		else if (!strcmp(pKd->GetName(), "ImageTexture"))
		{
			ImageTexture* pImageTexture = dynamic_cast<ImageTexture*>(pKd);

			ShowImageTextureProperty(pImageTexture, "Kd");
		}
	}
	else if( !strcmp( MaterialTypeName , "GlassMaterial" ) )
	{
		GlassMaterial* pGlassMaterial = dynamic_cast< GlassMaterial* >( pMaterial );

		// ------------------------------------------------------------------------------------
		Texture* pKr = pGlassMaterial->GetKr();

		if( !strcmp( pKr->GetName() , "ConstantTexture" ) )
		{
			ConstantTexture* pConstantTexture = dynamic_cast< ConstantTexture* >( pKr );

			ShowConstantTextureProperty( pConstantTexture , "kr" );
		}
		else if (!strcmp(pKr->GetName(), "ImageTexture"))
		{
			ImageTexture* pImageTexture = dynamic_cast<ImageTexture*>(pKr);

			ShowImageTextureProperty(pImageTexture, "Kr");
		}

		// ------------------------------------------------------------------------------------
		Texture* pKt = pGlassMaterial->GetKt();

		if( !strcmp( pKt->GetName() , "ConstantTexture" ) )
		{
			ConstantTexture* pConstantTexture = dynamic_cast< ConstantTexture* >( pKt );

			ShowConstantTextureProperty( pConstantTexture , "kt" );
		}
		else if (!strcmp(pKt->GetName(), "ImageTexture"))
		{
			ImageTexture* pImageTexture = dynamic_cast<ImageTexture*>(pKr);

			ShowImageTextureProperty(pImageTexture, "Kt");
		}

		// ------------------------------------------------------------------------------------
		Texture* pIOR = pGlassMaterial->GetRefraIndex();

		if( !strcmp( pIOR->GetName() , "ConstantTexture" ) )
		{
			ConstantTexture* pConstantTexture = dynamic_cast< ConstantTexture* >( pIOR );

			ShowConstantTextureProperty( pConstantTexture , "IOR" );
		}
		else if (!strcmp(pIOR->GetName(), "ImageTexture"))
		{
			ImageTexture* pImageTexture = dynamic_cast<ImageTexture*>(pIOR);

			ShowImageTextureProperty(pImageTexture, "IOR");
		}
		// ------------------------------------------------------------------------------------
	}
}

void ShowConstantTextureProperty( ConstantTexture* pConstantTexture , const char* PropertyName )
{
	Spectrum R = pConstantTexture->GetValue();

	float r[3] = { R[0] , R[1] , R[2] };

	if( ImGui::InputFloat3( PropertyName , r ) )
	{
		pConstantTexture->SetValue( Spectrum::FromRGB( ( float )r[0] , ( float )r[1] , ( float )r[2] ) );
	}
}

void ShowCheckboardTextureProperty( CheckboardTexture* pCheckboardTexture , const char* FirstPropertyName , const char* SecondPropertyName )
{
	Spectrum Value1 , Value2;
	pCheckboardTexture->GetValue( Value1 , Value2 );

	float r[3] = { Value1[0] , Value1[1] , Value1[2] };

	if( ImGui::InputFloat3( FirstPropertyName , r ) )
	{
		pCheckboardTexture->SetValue( Spectrum::FromRGB( ( float )r[0] , ( float )r[1] , ( float )r[2] ) , 1 );
	}

	float r2[3] = { Value2[0] , Value2[1] , Value2[2] };
	if( ImGui::InputFloat3( SecondPropertyName , r2 ) )
	{
		pCheckboardTexture->SetValue( Spectrum::FromRGB( ( float )r2[0] , ( float )r2[1] , ( float )r2[2] ) , 2 );
	}
}

void ShowImageTextureProperty( ImageTexture* pImageTexture , const char* PropertyName )
{
	char* pFilename = pImageTexture->GetFilename();

	if (ImGui::InputText(PropertyName, pFilename, strlen(pFilename)+1))
	{
		pImageTexture->SetFilename(pFilename);
	}
}