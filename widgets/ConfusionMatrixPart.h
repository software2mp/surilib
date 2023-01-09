/*! \file ConfusionMatrixPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CONFUSIONMATRIXPART_H_
#define CONFUSIONMATRIXPART_H_



// Includes Estandar
#include <map>
#include <string>

// Includes Suri
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "ClassMappingPart.h"

// Includes Wx

// Defines

namespace suri {

// forwards
class Element;
class DatasourceManagerInterface;

/**
 *
 */
class ConfusionMatrixPart : public suri::Part, public suri::ProcessAtributeProvider {
public:
   ConfusionMatrixPart(Element* pElement, DatasourceManagerInterface* pDatasourceManager);
   virtual ~ConfusionMatrixPart();
   /** Indica si el Part tiene cambios para salvar */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part */
   virtual bool CommitChanges();
   /** Restaura los valor's originales del Part */
   virtual bool RollbackChanges();
   /** Crea el Tool Window */
   bool CreateToolWindow();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Devuelve si la parte tiene datos validos */
   virtual bool HasValidData();
   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   typedef std::map<std::string, DatasourceInterface*> ResultMap;
   /** devuelve el mapa resultado en un mapa <nombreclase, recinto> **/
   std::vector<ResultMap> GetResultMap();

private:
   // Parche para que se pueda acceder a los datos desde el proceso. Refactorizar.
   ClassMappingPart* pClassMappingPart_;
   /** Asosiacion nombre-id para los recintos **/
   typedef std::map<std::string, DatasourceInterface*> EnclosureMap;
   EnclosureMap enclosureMap_;
   /** Metodo auxiliar para la carga del mapa y choice para el part de mapeo **/
   void LoadMap(ClassMappingPart::MappingTable& Map,
               ClassMappingPart::ChoiceValues& Choice);
   Element* pElement_;
   DatasourceManagerInterface* pDatasourceManager_;
   /** Indica si el contenido que se carga al mapeo clase-recinto es valido**/
   /** es false cuando se detecta algun recinto inicial no existente **/
   bool hasvaliddata_;
};

} /** namespace suri */

#endif /* CONFUSIONMATRIXPART_H_ */
