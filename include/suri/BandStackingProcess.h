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

#ifndef __BAND_STAC_PROCESS_H_
#define __BAND_STAC_PROCESS_H_

#include <vector>

#include "suri/FileExporterProcess.h"

namespace suri {

class BandStackingProcess : public FileExporterProcess {
   /** ctor copia **/
   BandStackingProcess(const BandStackingProcess&);

public:
   /** Nombre del proceso **/
   static const std::string kProcessName;
    /** Ctor. */
   BandStackingProcess(Element *pElement, const Subset &WindowSubset,
                       DataViewManager* pDataViewManager);

   /** Ctor. */
   BandStackingProcess(std::vector<Element*> InputElements, const Subset &ViewerSubset,
                       DataViewManager* pDataViewManager);
   virtual ~BandStackingProcess();

protected:
   /** 
    * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
    * agreguen sus atributos especificos
    * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
    * @return vector vacio en caso de que no tenga parametros adicionales
   **/
   virtual std::vector<wxXmlNode*> DoGetXmlParameters() const;
};

}  /** namespace suri **/
#endif   /** __BAND_STAC_PROCESS_H_ **/
