/*! \file ClassFussionParametersPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CLASSFUSSIONPARAMETERSPART_H_
#define CLASSFUSSIONPARAMETERSPART_H_

// Includes Estandar
#include <string>
#include <vector>
#include <map>
#include <utility>

// Includes Suri
#include "ClassificationPart.h"
#include "ClassFussionAlgorithm.h"
#include "ClassMappingPart.h"
#include "wxGenericTableBase.h"

// Includes Wx
#include "wx/event.h"

// Defines

// forwards

namespace suri {

/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(ClassFusionEvent);

/**
 *Part para la fusion de clases
 */
class ClassFussionParametersPart : public suri::ClassificationPart {
   ClassFussionParametersPart(const ClassFussionParametersPart&);

public:
   /** Constructor **/
   explicit ClassFussionParametersPart(Element* pElement);
   /** Destructor **/
   virtual ~ClassFussionParametersPart();
   /** Retorna las propiedades de los pixels que no tienen clase */
   virtual bool GetNonClassifiedPixelInfo(ClassInformation &Properties);
   /** Retorna el nombre del algoritmo que se usara para clasificar */
   virtual std::string GetAlgorithmName();
   /** Retorna un puntero al algoritmo de clasificacion creado por part */
   virtual ClassificationAlgorithmInterface* GetClassificationManager();
   /** Metodos Part */
   /** Indica si el Part tiene cambios para salvar */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Crea el Tool Window */
   bool CreateToolWindow();
   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   /** Retorna la cantidad de clases */
   virtual int GetClassCount();
   /** Retorna las propiedades de la clase en la posicion indicada */
   virtual bool GetClassProperties(std::vector<ClassInformation> &Properties);
   /** Retorna el nodo que se debe agregar al elemento que se renderiza */
   virtual wxXmlNode* GetClassificationRendererNode(RasterElement* pRasterElement);

private:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(ClassFusionEvent);
   /** Algoritmo */
   ClassFussionAlgorithm algorithm_;
   /** Elemento al cual pertenecen los datos del part */
   Element* pElement_;
   /** Part que realiza el mapeo para la fusion */
   ClassMappingPart* pClassMappingPart_;
   /** Metodo que asocia el evento de agregar clase al boton del part */
   void ConfigureButtonEvent();
   /** Evento que se ejecuta al hacer click sobre boton de agregar clase */
   void OnAddClassButtonEventHandler(wxCommandEvent &Event);
   /** Objeto para conectar */
   EVENT_OBJECT_PROPERTY(ClassFusionEvent);
   /** Convierte las clases del elemento en cluster para el algoritmo de clasificacion */
   bool InsertClassesToTable(ClassMappingPart::MappingTable &pMappingTable,
                             std::vector<ClassMappingPart::ChoiceValues> &pClassesMap);
   /** Convierte el contenido de la tabla en un nodo xml
    * para la configuracion del algoritmo*/
   bool TableToClustersXmlNode(const ClassMappingPart::MappingTable& MappingTable);
   /*** Mapa para relacion id-nombre*/
   typedef std::map<std::string, ClassInformation> ClassMapping;
   typedef std::pair<std::string, ClassInformation> ClassMappingPair;
   /** Mapa para la relacion nombre */
   ClassMapping classmapping_;
   /** Clases que son el resultado de la fusion */
   std::vector<ClassInformation> resultclasses_;
   /** Indice para la creacion de clases */
   ClassInformation::IndexType lastindex_;
};

} /** namespace suri */

#endif /* CLASSFUSSIONPARAMETERSPART_H_ */
