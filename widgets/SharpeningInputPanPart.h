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

#ifndef SHARPENINGINPUTPANPART_H_
#define SHARPENINGINPUTPANPART_H_

#include "SelectionPart.h"
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/SuriObject.h"
#include "suri/RasterElement.h"

namespace suri {

DECLARE_EVENT_OBJECT(SharpeningInputPanPartEvent);

class SharpeningInputPanPart : public suri::Part,
                               public suri::ProcessAtributeProvider,
                               public suri::InputBandListener {
public:
   FRIEND_EVENT_OBJECT(SharpeningInputPanPartEvent);

   /**
    * Ctor.
    */
   SharpeningInputPanPart(RasterElement* pElement, bool Enable = true, bool Modified = false);

   /**
    * Dtor.
    */
   virtual ~SharpeningInputPanPart();

   /**
    * Indica si el Part tiene cambios para salvar.
    */
   virtual bool HasChanged();

   /**
    * Salva los cambios realizados en el Part.
    */
   virtual bool CommitChanges();

   /**
    * Restaura los valores originales del Part.
    */
   virtual bool RollbackChanges();

   /**
    * Inicializa el part.
    */
   virtual void SetInitialValues();

   /**
    * Devuelve si la parte tiene datos validos.
    */
   virtual bool HasValidData();

   /**
    * Actualiza el estado de la parte.
    */
   virtual void Update();

   /**
    * Crea la ventana de la parte.
    */
   virtual bool CreateToolWindow();

   /**
    * Retorna el icono de la herramienta.
    */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

   /**
    * Metodo a implementar por la clase que recibe las notificaciones
    * para recibir la lista de bandas seleccionadas.
    */
   virtual void OnBandSelected(std::vector<BandInfo> BandIxsP);

private:
   EVENT_OBJECT_PROPERTY(SharpeningInputPanPartEvent);

   RasterElement* pElement_;
   BandInfo band_;
   bool changed_;
   int panix_;

   /**
    * Inicializa el textbox.
    */
   void InitializeTextBox();

   /**
    * Devuelve el elemento de alta resolucion.
    */
   RasterElement* GetHiResRasterElement(const std::vector<Element*>& Elements);
};

}  // namespace suri

#endif  // SHARPENINGINPUTPANPART_H_
