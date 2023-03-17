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

#ifndef CSVFORMATSELECTIONPART_H_
#define CSVFORMATSELECTIONPART_H_

// Includes Estandar
#include <string>
// Includes Suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
// Includes Wx
// Defines
// forwards

namespace suri {
// forwards
DECLARE_EVENT_OBJECT(CsvFormatSelectionEventHandler);

/*!
 * Clase que permite definir el archivo y formato de
 * salida para un archivo csv
 */
class CsvFormatSelectionPart : public suri::Part, public ProcessAtributeProvider {
   /** ctor copia **/
   CsvFormatSelectionPart(const CsvFormatSelectionPart&);
public:
   /** Constructor */
   CsvFormatSelectionPart();
   /** Destructor **/
   virtual ~CsvFormatSelectionPart();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();
   /** Devuelve el icono de la parte */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;
   /** Obtiene el formato elegido para el archivo de salida **/
   std::string GetFormat() const;
   /** Obtiene el nombre del archivo de salida **/
   std::string GetFileName() const;

private:
   /** Evento UIUpdate */
   void OnUiUpdate(wxUpdateUIEvent &Event);
   void OnButtonClicked(wxCommandEvent &Event);
   /** nombre del archivo de salida **/
   std::string fileName_;
   /** formato del archivo de salida **/
   std::string format_;
   FRIEND_EVENT_OBJECT(CsvFormatSelectionEventHandler);
   EVENT_OBJECT_PROPERTY(CsvFormatSelectionEventHandler);
};

} /** namespace suri */

#endif /* CSVFORMATSELECTIONPART_H_ */
