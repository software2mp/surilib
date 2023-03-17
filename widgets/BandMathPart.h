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

#ifndef BANDMATHPART_H_
#define BANDMATHPART_H_

// Includes standard
#include <map>
#include <vector>
#include <string>

// Includes Suri
#include "suri/PartCollection.h"
#include "EquationParser.h"
#include "wxGenericTableBase.h"
#include "UniversalLibraryNameEditionPart.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/LibraryItemSelectionInterface.h"
#include "suri/BasicLibraryItemSelector.h"
// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {

class EquationVariableMappingPart;
/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(BandMathPartEvent);

/**
 * Part que permite ingresar una ecuacion y asignarle las bandas
 * en la imagen de entrada a las variables.
 */
class BandMathPart : public Part,
                     public ProcessAtributeProvider,
                     public LibraryItemSelectionInterface {
private:
   static const std::string BAND_MATH_XRC_NAME; /*! Nombre de xrc de Part */
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(BandMathPartEvent);

public:
   /** Ctor */
   explicit BandMathPart(const std::vector<std::string> &BandNames, LibraryManager* pManager);
   /** Dtor */
   virtual ~BandMathPart();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Devuelve si la parte tiene datos validos */
   virtual bool HasValidData();
   /** Actualiza el estado del Part */
   virtual void Update();
   /** Se crea la ventana de herramienta en las clases derivadas */
   virtual bool CreateToolWindow();
   /** Retorna la ecuacion seleccionada por el usuario */
   std::string GetEquation();
   /** Retorna las bandas asignadas a cada variable */
   void GetVariableBands(std::map<std::string, int> &VariableBands);
   /** Retorna el tipo de dato de la salida */
   std::string GetOutputDataType();
   /**
    * Configura el editor con el item que se pasa por parametro. Se pasa por parametro
    * un boolean que indica si se trata de la primera seleccion (para que el part no piense
    * que posee cambios cuando en realidad es la seleccion inicial del CGU)
    */
   virtual void SetActiveItem(const LibraryItem* pItem);
   /*!
    *  Retorna un bitmap con el icono de la ventana.
    * Si se pasa solo X o solo Y el otro parametro tomara un valor que conserve
    * la relacion entre lados con la imagen.
    * Al final de ser necesario hace Rescale.
    * @param[out] ToolBitmap icono de la ventana
    * @param[in] X ancho del icono
    * @param[in] Y alto del icono
    */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;

private:
   // ---------------- Metodos que actualizan/leen la gui ----------------
   /** Actualiza los atributos widget con datos en control grafico */
   bool ReadParametersFromWidget();
   /** Actualiza el control grafico con datos en los atributos widget */
   bool UpdateParametersToWidget();
   /** Retorna la bandas seleccionada por el usuario para una variable */
   int GetVariableSelectedBand(const std::string &VariableName);

   // ---------------- Metodos que inicializan la gui --------------------
   /** Configura el choice con los tipos de dato */
   void ConfigureDataTypeChoice();

   // ----------------------- Metodos internos --------------------------
   /** Configura el tipo de dato de salida default */
   void SetInitialOutputDataType();

   // ----------------------------- Eventos -----------------------------
   /** Evento que se ejecuta al hacer click sobre checkbox utilizar biblioteca */
   void OnLibraryCheckboxClicked(wxCommandEvent &Event);
   std::vector<std::string> dataTypeNames_; /*! Nombre de tipo de datos ordenaados pos en choice */
   int widgetSelectedDataType_; /*! Tipo de dato en widget. */
   int selectedDataType_; /*! Tipo de dato a la salida. */
   EVENT_OBJECT_PROPERTY(BandMathPartEvent); /*! Objeto para conectar */
   BasicLibraryItemSelector* pItemSelector_; /*! Panel selector del item de biblioteca */
   EquationVariableMappingPart* pBandMappingPart_;
};

}  // namespace suri

#endif /* BANDMATHPART_H_ */
