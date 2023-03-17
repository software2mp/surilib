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

#ifndef CLASSFUSSIONALGORITHM_H_
#define CLASSFUSSIONALGORITHM_H_

// Includes Estandar
#include <string>
#include <vector>
#include <map>
#include <utility>

// Includes Suri
#include "ClassificationAlgorithmInterface.h"

// Includes Wx

// Defines

// forwards
class wxXmlNode;

namespace suri {

class ClassFussionAlgorithm : public suri::ClassificationAlgorithmInterface {
   ClassFussionAlgorithm(const ClassFussionAlgorithm&);

public:
   ClassFussionAlgorithm();
   /** dtor */
   virtual ~ClassFussionAlgorithm();
   /** Definicion de mapa de asignacion de clases */
   /** key = indice clase a fusionar , value = indice destino **/
   typedef long IndexValue;
   typedef std::map<IndexValue, IndexValue> FussionClasesTables;
   /** key = indice clase a fusionar , value = indice destino **/
   typedef std::pair<IndexValue, IndexValue> FussionClasesTablesPair;
   void SetFussionTable(const FussionClasesTables &FussionTable);
   /** Actualiza los datos internos del algoritmo con los del nodo. */
   virtual bool Update(const wxXmlNode* pAlgorithmNode);
   /** Genera nodo con los datos internos del algoritmo. */
   virtual wxXmlNode* GetNode();
   /** Clasifica los datos de entrada */
   virtual bool Classify(int* pDest, std::vector<void*> pSource, size_t Size,
                         const std::string &DataType);
   /** Verifica que el algoritmo este bien configurado */
   virtual bool Check() const;
   /** */
   int GetClassCount() const;
   /** Indice para la noclase **/
   static const int NoClusterIndex;

private:
   /** Metodo para la iniciacion de la instancia **/
   virtual ClassFussionAlgorithm* Initialize(const std::string&);
   /** Valida el contenido de los nodos cluster y nocluster **/
   bool IsClusterNodeValid(const wxXmlNode* pNode, bool IsNoCluster = false);
   /** Metodo auxiliar para la validacion del xml*/
   bool IsXmlValid(const wxXmlNode* pNode);
   /** Tabla de asignacion de clases **/
   FussionClasesTables fussionTable_;
   /** Convierte el map seteado al xml **/
   bool FussionMapToXml(wxXmlNode* pParentNode);
   /** Convierte el xml a el mapea de asignacion**/
   bool XmlToMapFussion(const wxXmlNode* pNode);
   /* A partir del nodo cluster crea la informacion para insertar al mapa de fusion */
   bool InsertFussionMapFromXml(const wxXmlNode* pNode);
};

} /** namespace suri */

#endif /* CLASSFUSSIONALGORITHM_H_ */
