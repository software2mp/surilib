/*! \file LutSelectionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LUTSELECTIONPART_H_
#define LUTSELECTIONPART_H_

// Includes standar
#include <list>

// Includes suri
#include "ChoiceSelectionPart.h"
#include "suri/LutArray.h"
#include "LutRenderer.h"

// Includes wx
#include "wx/wx.h"

/** namespace suri */
namespace suri {
/**
 * Permite seleccionar entre varias lut y modificar el nodo del elemento
 * correspondiente. Como puedo tener varios tipos de LUTs en el elemento tiene
 * metodos virtuales que le permiten encontrar la lut correcta y mostrar el
 * listado de luts pertinente
 */
class LutSelectionPart : public ChoiceSelectionPart {
public:
   /** Ctor legacy */
   explicit LutSelectionPart(RasterElement * pElement);
   /** Ctor */
   explicit LutSelectionPart(RasterLayer * pLayer);
   /** Dtor */
   virtual ~LutSelectionPart();

protected:
   /** Actualiza el contenido del choice. */
   bool ResetList();
   /** Actualiza el nodo LUT correspondiente en el elemento. */
   bool ConfigureElement();
   /** Si encuentra un nodo lut en el elemento lo agrega al choice */
   bool AddElementLutToChoice();
   /** Busca si en el elemento hay una LUT de tipo correcto */
   bool GetLutNode(wxXmlNode* &pLutNode);
   /** Remplaza/agrega la lut en el nodo renderizacion del elemento. */
   bool SetLutNode(wxXmlNode* pLutNode);
   /** Retorna una lista con los nombres de todas las luts soportadas */
   virtual std::list<std::string> GetLutNames()=0;
   /** Genera una Lut soportada por el sistema. */
   virtual bool CreateLutNode(const std::string &LutName, bool Active,
                              wxXmlNode* &pNewLutNode)=0;
   /** Genera una Lut modificando la que se pasa por parametro. */
   virtual bool CreateLutNode(const wxXmlNode* pLutNode, bool Active,
                              wxXmlNode* &pNewLutNode)=0;
   /** Busca si el nodo coincide con una lut soportada y retorna el nombre. */
   virtual bool GetElementLutName(const wxXmlNode* pLutNode, std::string &LutName)=0;
   /** Verifica si el nodo lut es de tipo correcto. */
   virtual bool IsValidLutType(const wxXmlNode* pLutNode)=0;
   /** Retorna el estado de activacion del elemento */
   virtual bool IsActive(const wxXmlNode* pLutNode)=0;
   /** Retorna el estado de activacion del elemento */
   virtual std::string GetNodeName()=0;
   /** Si el elemento seleccionado no es soportado inactiva boton refresh */
   virtual void OnUpdate();

   /** Busca la posicion de una lut en suportedLuts_ usando el nombre. */
   bool GetLutPositionInChoice(const std::string &Name, int &Position);

   std::vector<std::string> supportedLuts_; /*! vector con los nombres de las */
   /* luts soportadas. No estan traducidas. */
};
}

#endif /* LUTSELECTIONPART_H_ */
