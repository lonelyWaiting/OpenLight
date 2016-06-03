#pragma once

class Scene;
class Renderer;
class Texture;
class ConstantTexture;
class CheckboardTexture;
class Material;

void ShowPropertyWindow( Scene* scene , Renderer* renderer , int index , bool& bShow );

void ShowMaterialProperty( Material* pMaterial , const char* MaterialTypeName );

void ShowConstantTextureProperty( ConstantTexture* pConstantTexture , const char* PropertyName );

void ShowCheckboardTextureProperty( CheckboardTexture* pCheckboardTexture , const char* FirstPropertyName , const char* SecondPropertyName );