/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
