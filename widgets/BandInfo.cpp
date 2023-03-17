/*! \file BandInfo.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>

// Includes Suri
#include "BandInfo.h"
#include "suri/RasterElement.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/XmlFunctions.h"
// Includes Wx
#include "wx/xml/xml.h"
#include <wx/sstream.h>
// Defines
// forwards

namespace suri {

/**
 * Ctor. default
 */
BandInfo::BandInfo(){}

/**
 * Ctor.
 */
BandInfo::BandInfo(suri::RasterElement* pElementP, int PositionP,
                   const std::string& NameP, int Width, int Height,
                   const std::string& TypeP) {
   pElement_ = pElementP;
   position_ = PositionP;
   name_ = NameP;
   width_ = Width;
   height_ = Height;
   type_ = TypeP;
}

BandInfo::BandInfo(const BandInfo& bandInfo){
   this->pElement_ = bandInfo.pElement_;
   this->position_ = bandInfo.position_;
   this->name_ = bandInfo.name_;
   this->width_ = bandInfo.width_;
   this->height_ = bandInfo.height_;
   this->type_ = bandInfo.type_;
}

/**
 * Dtor
 */
BandInfo::~BandInfo() {
}

/**
 * Devuelve el elemento al que pertenece la banda.
 */
suri::RasterElement* BandInfo::GetElement() const{
   return pElement_;
}

/**
 * Devuelve la posicion de la banda en el elemento.
 */
int BandInfo::GetPosition() const{
   return position_;
}

/**
 * Devuelve el nombre de la banda (junto con el nombre del elemento)
 */
std::string BandInfo::GetName() const{
   return name_;
}

/**
 * Devuelve el ancho de la banda.
 */
int BandInfo::GetWidth() const{
   return width_;
}

/**
 * Devuelve el alto de la banda.
 */
int BandInfo::GetHeight() const{
   return height_;
}

/**
 * Devuelve el tipo de la banda.
 */
std::string BandInfo::GetType() const{
   return type_;
}

/**
 *
 */
std::string BandInfo::ExtractType(suri::RasterElement* pElement) {
   return pElement->GetRasterProperties().DataType;
}

wxXmlNode* BandInfo::SerializeXml() const{
	wxXmlNode* pXmlRoot = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(BAND_NODE));
   pXmlRoot->AddChild(CreateKeyValueNode(
		 BAND_POSITION, NumberToString<int>(this->position_)));
   pXmlRoot->AddChild(CreateKeyValueNode(
		 NAME_NODE, _(this->name_.c_str())));
   pXmlRoot->AddChild(CreateKeyValueNode(
         WIDTH_NODE, NumberToString<int>(this->width_)));
   pXmlRoot->AddChild(CreateKeyValueNode(
         HEIGHT_NODE, NumberToString<int>(this->height_)));
   pXmlRoot->AddChild(CreateKeyValueNode(
		 TYPE_NODE, _(this->type_.c_str())));
   return pXmlRoot;
}

BandInfo* BandInfo::DeserializeXml(wxXmlNode* pRoot){
   wxXmlNode* pChild = pRoot->GetChildren();
   //Deserializo asumiendo que se mantienen en el orden que los guardó
   int position = Serializable::DeserializeInt(pChild, BAND_POSITION);
   pChild = pChild->GetNext();
   std::string name = Serializable::DeserializeString(pChild, NAME_NODE);
   pChild = pChild->GetNext();
   int width = Serializable::DeserializeInt(pChild, WIDTH_NODE);
   pChild = pChild->GetNext();
   int height = Serializable::DeserializeInt(pChild, HEIGHT_NODE);
   pChild = pChild->GetNext();
   std::string type = Serializable::DeserializeString(pChild, TYPE_NODE);
   BandInfo* bandInfo = new BandInfo(0, position, name, width, height, type);
   return bandInfo;
}

} /** namespace suri */
