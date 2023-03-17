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

#ifndef VIEWPORTINTERFACE_H_
#define VIEWPORTINTERFACE_H_

// Includes Estandar
// Includes Suri
#include "suri/SuriObject.h"
#include "suri/ViewcontextManagerInterface.h"

// Includes Wx
// Defines
/** Macro que castea al viewport a un viewer **/
#define AS_VW(ViewportInterface) (dynamic_cast<suri::ViewerWidget*>(ViewportInterface))



namespace suri {
// forwards
class CoordinatesTransformation;

/*!
 *  Interface que representa el comportamiento de un viewport.
 */
class ViewportInterface {
   /** ctor copia **/
   ViewportInterface(const ViewportInterface&);

public:
   /** constructor **/
   ViewportInterface() {
   }
   /** destructor **/
   virtual ~ViewportInterface() {
   }
   /** Obtiene el id asociado al viewport **/
   virtual SuriObject::UuidType GetId() const=0;
   /** Obtiene el id entero asociado al viewport**/
   virtual SuriObject::UuidIntType GetIntId() const=0;
   /** Obtiene el id del viewcontext asociado**/
   virtual SuriObject::UuidType GetViewcontextUuid() const=0;
   /** Indica/asocia el viewcontext a utilizar **/
   virtual bool SetViewcontext(ViewcontextInterface *pViewcontext)=0;
   /** Obtiene la transformacion asociada **/
   virtual CoordinatesTransformation* GetTransformation()=0;

protected:
private:
};

} /* namespace suri */

#endif /* VIEWPORTINTERFACE_H_ */
