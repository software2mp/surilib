/*! \file SpectralSignEditionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
