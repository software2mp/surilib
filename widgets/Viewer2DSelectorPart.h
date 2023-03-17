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

#ifndef VIEWER2DSELECTOR_H_
#define VIEWER2DSELECTOR_H_

#include <map>
#include <string>
#include <sstream>
#include <vector>

#include "suri/Part.h"
#include "suri/SuriObject.h"

namespace suri {

DECLARE_EVENT_OBJECT(Viewer2DSelectorPartEvent);

class Viewer2DSelectorPart : public Part {
public:
   FRIEND_EVENT_OBJECT(Viewer2DSelectorPartEvent);

   static const std::string kViewer;
   static const int kInitialBasePos = 0;
   static const int kInitialWarpPos = 1;


   /**
    * Ctor
    */
   Viewer2DSelectorPart(bool Enable = true, bool Modified = false);

   /**
    * Dtor
    */
   virtual ~Viewer2DSelectorPart();

   /**
    * Indica si el Part tiene cambios para salvar
    */
   virtual bool HasChanged();

   /**
    * Salva los cambios realizados en el Part.
    */
   virtual bool CommitChanges();

   /**
    * Restaura los valor's originales del Part.
    */
   virtual bool RollbackChanges();

   /**
    * Inicializa el part
    */
   virtual void SetInitialValues();

   /**
    * Actualiza el estado de la parte
    */
   virtual void Update();

   /**
    * Modifica el nombre del viewer.
    */
   virtual bool ConfigureWidget();

   /**
    * Devuelve si la parte tiene datos validos
    */
   virtual bool HasValidData();

   /**
    * Configura los visualizadores disponibles.
    */
   void SetCurrentViewers(const std::map<SuriObject::UuidType, std::string>& CurrentViewers);

   /**
    * Obtiene los visualizadores seleccionados.
    */
   std::vector<SuriObject::UuidType> GetSelectedViewers() const;

   /**
    * Handler para el evento de cambio de seleccion de choice base.
    */
   virtual void OnChBaseSelectionChanged(wxCommandEvent &Event);

   /**
    * Handler para el evento de cambio de seleccion de choice warp.
    */
   virtual void OnChWarpSelectionChanged(wxCommandEvent &Event);

protected:
   EVENT_OBJECT_PROPERTY(Viewer2DSelectorPartEvent);

   int baseSelection_;
   int warpSelection_;
   SuriObject::UuidType baseId_;
   SuriObject::UuidType warpId_;

   std::map<SuriObject::UuidType, std::string> currentViewers_;

   /**
    * Inicializa el part realizando todas las configuraciones necesarias.
    */
   void InitializePart();

   /**
    * Inicializa los controles choice para la seleccion de visualizadores.
    */
   void InitializeChoices();

   /**
    * Inicializa el control choice para visualizador base.
    */
   void InitializeChoiceBase();

   /**
    * Inicializa el control choice para visualizador warp.
    */
   void InitializeChoiceWarp();

   /**
    * Inicializacion general.
    */
   virtual void Initialize();

   /**
    * Verifica si los visualizadores seleccionados son distintos. De no serlos
    * muestra un mensaje al usuario.
    */
   bool AreEqualChoices() const;
   /**
    * Redibujo panel padre
    */
   void RefreshContainer();
};

}  // namespace suri

#endif  // VIEWER2DSELECTOR_H_
