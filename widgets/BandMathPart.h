/*! \file BandMathPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
