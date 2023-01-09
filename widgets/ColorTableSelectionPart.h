/*! \file ColorTableSelectionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COLORTABLESELECTIONPART_H_
#define COLORTABLESELECTIONPART_H_

// Includes estandar
#include <vector>
#include <string>
#include <list>

// Includes suri
#include "LibraryItemEditorPart.h"
#include "suri/RasterElement.h"
#include "RasterLayer.h"
#include "suri/SuriObject.h"

/** namespace suri */
namespace suri {

// Forwards
class RasterLayer;
class BandCombinationAndEnhancementPart;

DECLARE_EVENT_OBJECT(ColorTableEventHandler);

/**
 * Permite seleccionar entre varias tablas de colores soportadas.
 */
class ColorTableSelectionPart : public LibraryItemEditorPart {
   /** ctor copia**/
   ColorTableSelectionPart(const ColorTableSelectionPart&);

public:
   /** Ctor legacy*/
   ColorTableSelectionPart(BandCombinationAndEnhancementPart* pBandCombPart,
                           LibraryManager* pManager,
                           const SuriObject::UuidType& LibraryId,
                           RasterElement * pElement);
   /** Ctor*/
   ColorTableSelectionPart(BandCombinationAndEnhancementPart* pBandCombPart,
                           LibraryManager* pManager,
                           const SuriObject::UuidType& LibraryId, RasterLayer * pLayer);
   /** Dtor */
   virtual ~ColorTableSelectionPart();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Metodo que activa o desactiva los componontes que contiene el Part **/
   virtual void Activate(bool Activate);
   /** Metodo que tilda/destilda el check */
   void Check(bool Activate);

private:
   FRIEND_EVENT_OBJECT(ColorTableEventHandler);EVENT_OBJECT_PROPERTY(ColorTableEventHandler);
   /** Deshabilita la herramienta de realces. */
   void OnColorTableSelectionCheck(wxCommandEvent &Event);
   /** Genera una Lut soportada por el sistema. */
   virtual bool CreateLutNode(const std::string &LutName, bool Active,
                              wxXmlNode* &pNewLutNode);
   /**  Metodo que obtiene a partir de los datos del editor, un vector con los atributos
    *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
    *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
    *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
    *   @return vector con atributos
    *   @return NULL en caso de que la informnacion del part no sea valida
    *   **/
   virtual std::vector<LibraryItemAttribute*>* CreateAttributesFromFields();
   /**
    *  Metodo auxiliar privado que configura el part en si para cada editor en particular.
    *  Este metodo es el que realmente tienen que implementar las clases hijas para
    * lograr la especializacion a la hora de iniciar el part (metodo que se llama
    * en CreateToolWindow, a modo de Template Method).
    **/
   virtual bool ConfigureFeatures();
   /**
    * Metodo auxiliar privado que es necesario implementar en las clases hijas.
    * Este metodo es el encargado de actualizar realmente la informacion en el part
    * Es llamado en el metodo SetActiveItem a modo de Template Method
    **/
   virtual void UpdateFields(const LibraryItem* pItem, bool Modifiable = true);
   /**
    * Metodo auxiliar privado que agrega comportamiento adicional al commit de la
    * clase base. Este comportamiento adicional es necesario cuando
    * el commit involucra objetos adicionales a los items de biblioteca
    * del editor (ej. reproyeccion, en donde se necesita actualizar los params
    * de reproyeccion ademas de leer el item de biblioteca)
    * @return true en caso de realizar correctamente el commit "interno"
    * @return false en caso de que ocurra algun error durante el commit
    */
   virtual bool OnCommitChanges();
   /** Metodo que activa o desactiva el uso de tabla de colores **/
   void ActivateSelection(bool Activate);
   /** bool que indica el ultimo estadod de activacion **/
   bool activate_;
   /** Part que contiene al part de tabla de color. Se utiliza para notificar la activacion
    *  del uso de tabla de colores **/
   BandCombinationAndEnhancementPart* pBandCombPart_;
   /** Capa de donde se muestra la informacion **/
   RasterLayer * pLayer_;
   /** elemento raster de donde se muestra la informacion **/
   RasterElement * pElement_;
   LibraryManager* pManager_;
};
} /** namespace suri **/

#endif /* COLORTABLESELECTIONPART_H_ */
