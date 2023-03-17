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

// Includes standard
#include <vector>
#include <string>

// Includes Suri
#include "suri/PartCollection.h"
#include "LibraryTableEditionPart.h"

// Includes Wx
// Defines
#ifndef SPECTRALSIGNEDITIONPART_H_
#define SPECTRALSIGNEDITIONPART_H_

namespace suri {

// Forwards
class Table;
class SpectralSignItemAttribute;
class wxGenericTableBase;
class TablePart;
DECLARE_EVENT_OBJECT(SpectralSignTableEventHandler);

/**
 * Clase que representa un editor de items de biblioteca de firmas espectrales.
 */
class SpectralSignEditionPart: public suri::LibraryTableEditionPart {
   /** Nombre del XRC que representa la ventana. */
   static const std::string SPECTRAL_SIGN_EDITION_XRC_NAME;
   /** Nombre del XRC que representa el panel del cgu */
   static const std::string SPECTRAL_SIGN_CGU_XRC_NAME;
	/** Constructor Copia **/
	SpectralSignEditionPart(const SpectralSignEditionPart&);

public:
	/** Constructor **/
	SpectralSignEditionPart(const SuriObject::UuidType& LibraryId,
			LibraryManager* pManager,
			UniversalGraphicalComponentPartInterface::FeatureCombinationEnum Feature);
	/** Destructor **/
	virtual ~SpectralSignEditionPart();

private:
	/** Columna de longitud de onda */
	static const int WAVELENGTH_COL = 0;
	static const int AMPLITUDE_COL = 1;
	static const int REFLECTANCE_COL = 2;
	/** Metodo auxiliar que carga el contenido del item complejo seleccionado en la tabla del Part */
	virtual void LoadItemToTable(const LibraryItem* pItem, bool AddRangeToItem = false);
	/** Agrega un item complejo a la tabla */
	void AddValuesToTable(const SpectralSignItemAttribute* pComplex);
	/** Obtiene de la tabla los items complejos cargados y crea un item asociado */
	virtual void LoadAttributesFromTable(std::vector<LibraryItemAttribute*>* &pAttributes);
	/** Agrega las columnas de la tabla */
	virtual void AppendColumns();
   /**
    *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
    *  Este metodo es el que realmente tienen que implementar las clases hijas para
    *  lograr la especializacion a la hora de iniciar el part (metodo que se llama
    *  en CreateToolWindow, a modo de Template Method).
    **/
   virtual bool ConfigureFeatures();
   /** Metodo que se llama cuando se presiona el boton de agregar columna */
   virtual void OnAddRowClick(wxCommandEvent& Event);
   /** Metodo que se llama cuando se presiona el boton de eliminar columna */
   virtual void OnDeleteRowClick(wxCommandEvent& Event);
   /** Metodo auxiliar privado que configura las opciones disponibles en los choices */
   void ConfigureChoices();
   /** Metodo que se llama cuando se presiona el choice de longitud de onda */
   void OnChoiseWavelengthClicked(wxCommandEvent& Event);
   /** Metodo que se llama cuando se presiona el choice de longitud de magnitud */
   void OnChoiseMagnitudeClicked(wxCommandEvent& Event);
   FRIEND_EVENT_OBJECT(SpectralSignTableEventHandler);
   EVENT_OBJECT_PROPERTY(SpectralSignTableEventHandler);
   /** Item en edicion **/
   const LibraryItem* pCurrentItem_;
   /** Indice de choice de longitud de onda */
   int wavelengthindex_;
   /** Indice de choice de magnitud */
   int magnitudeindex_;
};
}

#endif /* SPECTRALSIGNEDITIONPART_H_ */
