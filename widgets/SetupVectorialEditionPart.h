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

#ifndef SETUPVECTORIALEDITION_H_
#define SETUPVECTORIALEDITION_H_

#include <vector>

#include "suri/Part.h"
#include "SimpleVectorStylePart.h"
#include "suri/DataViewManager.h"
namespace suri {

/** Clase encargada de la configuracion de la visualizacion de la
 *  edicion vectorial de la aplicacion 
 **/
class SetupVectorialEditionPart : public Part {
public:
   /** Ctor. */
   SetupVectorialEditionPart(DataViewManager* pDataViewManager);
   /** Dtor. */
   virtual ~SetupVectorialEditionPart();
   /** Crea el part (el interior de la ventana)*/
   virtual bool CreateToolWindow();
   /** Asigna el icono para el part.  */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Inicializa los controles */
   virtual void SetInitialValues();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Escribe el nuevo estilo en el archivo de configuracion. */
   void WriteNewStyles();
   friend class SimpleVectorStylePart;
private:
   std::vector<std::string> parameters_;
   std::vector<std::string> panels_;
   // Parts
   std::vector<ui::SimpleVectorStylePart*> styles_;
   // Estilos.
   std::string creationStyle_;
   std::string deletingStype_;
   std::string editionStyle_;
   std::string selectionStyle_;
   DataViewManager* pDataViewManager_;

};

} /* namespace suri */

#endif /* SETUPVECTORIALEDITION_H_ */
