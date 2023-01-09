/*! \file LibraryItemAttributeFactory.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */



// Includes Estandar
#include <string>
#include <vector>

// Includes Suri
#include "suri/LibraryItemAttributeFactory.h"
#include "suri/LibraryItemAttribute.h"
#include "suri/RangeItemAttribute.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/LibraryLinkItemAttribute.h"
#include "suri/SpectralSignItemAttribute.h"
#include "suri/ConvolutionFilterItemAttribute.h"

// Includes Wx
// Defines
// forwards

namespace suri {
/** String que describe los atributos de tipo rango **/
const std::string LibraryItemAttributeFactory::RangeString = "range";
/** String que describe los atributos de tipo link de biblioteca **/
const std::string LibraryItemAttributeFactory::LibLinkString = "liblink";
/** String que describe los atributos de tipo firma espectral **/
const std::string LibraryItemAttributeFactory::SpectralSignString = "spectralsign";
/** String que describe los atributos complejos **/
const std::string LibraryItemAttributeFactory::ComplexString = "complex";
/** String que describe los atributos de tipo filtros de convolucion */
const std::string LibraryItemAttributeFactory::ConvolutionFilterString = "filter";

/** constructor **/
LibraryItemAttributeFactory::LibraryItemAttributeFactory() {
}

/** destructor **/
LibraryItemAttributeFactory::~LibraryItemAttributeFactory() {
}

/** A partir del atributo que se pasa por parametro,
 * crea una reinterpretacion del mismo como un atributo de rango.
 * Formato atributo rango: "{nombre,inicio,fin,color}"
 * @param[in] pAttribute atributo que se desea interpretar como rango
 * @return nueva instancia de RangeItemAttribute con la informacion que contenia el atributo
 * @return NULL en caso de no poder reinterpretar el atributo como rango*/
RangeItemAttribute* LibraryItemAttributeFactory::CreateRangeItemAttribute(
      LibraryItemAttribute* pAttribute) {
   // replicar lo de firmas espectrales
   if (!pAttribute)
      return NULL;
#ifdef __UNUSED_CODE__
   RangeItemAttribute* pattr = NULL;
   std::string value = pAttribute->GetValue();
   std::vector < std::string > *pvalues = ParseComplexAttribute(
         pAttribute->GetValue(), RangeItemAttribute::StartAttributeChar,
         RangeItemAttribute::FinishAttributeChar, RangeItemAttribute::AtributesToken);
   if (pvalues) {
      for (unsigned int i = 0; i < pvalues->size(); ++i) {
         std::string value = pvalues->at(i);
         switch (i) {
            case RangeItemAttribute::RangeNamePosition: {
               pattr = new RangeItemAttribute(pAttribute->GetName(), value);
               break;
            }
            case RangeItemAttribute::StartValuePosition: {
               if (pattr)
                  pattr->SetStartValue(RangeDoubleToString(value));
               break;
            }
            case RangeItemAttribute::FinishValuePosition: {
               if (pattr)
                  pattr->SetFinishValue(RangeDoubleToString(value));
               break;
            }
            case RangeItemAttribute::HexaColorPosition: {
               if (pattr) {
                  std::string hexacolor = value;
                  pattr->SetHexaColor(hexacolor);
               }
               break;
            }
            default:
               break;
         }
      }
      delete pvalues;
   }
   return pattr;
#endif
   return new RangeItemAttribute(pAttribute->GetName(), pAttribute->GetValue());
}





/** Metodo que verifica si el atributo es del tipo rango
 *  @param[in] pAttribute atributo a verificar
 *  @return true en caso de que el atributo sea de tipo rango
 *  @return false en caso contrario **/
bool LibraryItemAttributeFactory::IsRangeItemAttribute(const LibraryItemAttribute* pAttribute) {
   // TODO: Migrar a RangeItemAttribute??
   if (!pAttribute)
      return false;
   return (pAttribute->GetName().find(RangeString) != std::string::npos);
}

/** A partir del atributo que se pasa por parametro,
 * crea una reinterpretacion del mismo como un atributo filtro de convolucion
 */
ConvolutionFilterItemAttribute* LibraryItemAttributeFactory::CreateConvolutionFilterItemAttribute(
      const LibraryItemAttribute* pAttribute, int Dimension) {
   if( !pAttribute)
      return NULL;
   return new ConvolutionFilterItemAttribute(pAttribute->GetName(), pAttribute->GetValue(), Dimension);
}
/** Metodo que verifica si el atributo es del tipo filtro de convolucion **/
bool LibraryItemAttributeFactory::IsConvolutionFilterItemAttribute(
      const LibraryItemAttribute* pAttribute) {
   return (pAttribute->GetName().find(ConvolutionFilterString) != std::string::npos);
}


/** Metodo que reinterpreta un atributo como uno de link a biblioteca
 * generando una instancia de la nueva interpretacion*/
LibraryLinkItemAttribute* LibraryItemAttributeFactory::CreateLibLinkAttribute(
      const LibraryItemAttribute* pAttribute) {
   if (!pAttribute)
      return NULL;
   LibraryLinkItemAttribute* pattr = NULL;
#ifdef __UNUSED_CODE__
   std::string value = pAttribute->GetValue();
   std::vector < std::string > *pvalues = ParseComplexAttribute(pAttribute->GetValue());
   if (pvalues) {
      std::string id;
      std::string libcode;
      for (unsigned int i = 0; i < pvalues->size(); ++i) {
         std::string value = pvalues->at(i);
         switch (i) {
            case LibraryLinkItemAttribute::ItemIdPosition:
               id = value;
               break;
            case LibraryLinkItemAttribute::LibraryCodePosition:
               libcode = value;
               break;
         }
      }
      if (!id.empty() && !libcode.empty()) {
         pattr = new LibraryLinkItemAttribute(id, libcode);
      }
      delete pvalues;
   }
#endif
   return pattr;
}

/** A partir del atributo que se pasa por parametro,
 * crea una reinterpretacion del mismo como un atributo complejo.
 * Formato atributo complejo: "{longitud de onda,amplitud,reflectancia}"
 * @param[in] pAttribute atributo que se desea interpretar como rango
 * @return nueva instancia de SpectralSignItemAttribute con la informacion que contenia el atributo
 * @return NULL en caso de no poder reinterpretar el atributo complejo */
SpectralSignItemAttribute* LibraryItemAttributeFactory::CreateSpectralSignItemAttribute(
      const LibraryItemAttribute* pAttribute) {
   if (!pAttribute)
      return NULL;
   return new SpectralSignItemAttribute(pAttribute->GetName(), pAttribute->GetValue());
}
} /** namespace suri */
