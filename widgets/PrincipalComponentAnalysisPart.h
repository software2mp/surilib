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

#ifndef PRINCIPALCOMPONENTANALYSISPART_H_
#define PRINCIPALCOMPONENTANALYSISPART_H_

#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/RasterElement.h"

namespace suri {

DECLARE_EVENT_OBJECT(PrincipalComponentAnalysisPartEvent);

class PrincipalComponentAnalysisPart : public Part, public ProcessAtributeProvider {
public:
   FRIEND_EVENT_OBJECT(PrincipalComponentAnalysisPartEvent);

   /**
    * Ctor.
    */
   PrincipalComponentAnalysisPart(RasterElement* pElement, bool Enable = true, bool Modified = false);

   /**
    * Dtor.
    */
   virtual ~PrincipalComponentAnalysisPart();

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
    * Implementacion del metodo de creacion de ventana de Widget.
    */
   virtual bool CreateToolWindow();
   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

private:
   EVENT_OBJECT_PROPERTY(PrincipalComponentAnalysisPartEvent);

   RasterElement* pElement_;
   bool changed_;

   /**
    * Handler para la seleccion del radio button correlacion.
    */
   void OnRadCorrelationSelected(wxCommandEvent& Event);

   /**
    * Handler para la seleccion del radio button covarianza.
    */
   void OnRadCovarianceSelected(wxCommandEvent& Event);

   /**
    * Handler para la seleccion del radio button total.
    */
   void OnRadTotalSelected(wxCommandEvent& Event);

   /**
    * Handler para la seleccion del radio button parcial.
    */
   void OnRadPartialSelected(wxCommandEvent& Event);

   /**
    * Habilita o deshabilita el control para la carga
    * de componentes principales.
    */
   void EnablePartialCount(bool Enable = true);
};

}  // namespace suri

#endif  // PRINCIPALCOMPONENTANALYSISPART_H_
