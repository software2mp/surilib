/*! \file TransformationModelParametersPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TRANSFORMATIONMODELPARAMETERSPART_H_
#define TRANSFORMATIONMODELPARAMETERSPART_H_


// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
#include "suri/ProcessAtributeProvider.h"
#include "LibraryItemEditorPart.h"
#include "WarpParametersPart.h"
#include "UniversalGraphicalComponentPart.h"
#include "suri/SuriObject.h"
#include "suri/LibraryManager.h"
#include "suri/CoordinatesTransformInterface.h"
#include "ReprojectionProcess.h"
#include "suri/TransformationFactory.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/Subject.h"
// Includes Wx

// Defines

// forwards

namespace suri {
// forwards
class GcpList;
class ParserResult;
/*!
 * Clase encargada de configurar los diferentes parametros de
 *  la transformacion
 */
class TransformationModelParametersPart : public ProcessAtributeProvider,
                                          public LibraryItemEditorPart,
                                          public Subject {
   /** ctor copia **/
   TransformationModelParametersPart(const TransformationModelParametersPart&);

public:
   /** Definicion de flags para permitir diferentes tipos de transformaciones **/
   static const int GcpFile = 0x01;
   static const int Polynomial = 0x02;
   static const int Exact = 0x04;
   /** ctor */
   explicit TransformationModelParametersPart(
         const SuriObject::UuidType& LibraryId, LibraryManager* pManager,
         ReprojectionProcess::ReprojectionParameters *pReprojectionParameters,
         const int TransformationFlags = Polynomial | Exact);
   virtual ~TransformationModelParametersPart();
   /**  Metodo que obtiene a partir de los datos del editor, un vector con los atributos
    *   Este metodo es el que hace el comportamiento particular de cada editor a la hora
    *   de obtener la informacion que posee y transmitirla al sistema de bibliotecas.
    *   Es utilizado a modo de Template Method (en CommitChanges y en el notificador)
    *   @return vector con atributos
    *   @return NULL en caso de que la informnacion del part no sea valida
    *   **/
   virtual std::vector<LibraryItemAttribute*>* CreateAttributesFromFields();
   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   /** Indica si el Part tiene cambios para salvar */
   virtual bool HasChanged();
   /** Inicializa el part */
   void SetInitialValues();
   /** Devuelve si la parte tiene datos validos */
   virtual bool HasValidData();
   /** Verifica los datos  antes de salir del part */
   virtual bool CommitChanges();
private:
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
   /**
    * Metodo auxiliar privado que agrega comportamiento adicional al rollback de la
    * clase base. Este comportamiento adicional es necesario cuando
    * el rollback involucra objetos adicionales a los items de biblioteca
    * del editor (ej. parts compuesto en donde se necesita llamar a los rollbacks de
    * los mismos)
    * @return true en caso de realizar correctamente el rollback "interno"
    * @return false en caso de que ocurra algun error durante el rollback
    */
   virtual bool OnRollback();
   /** Metodo auxiliar que a partir de un sistema de referencia genera la
    *  transformacion de coordenadas correspondiente
    */
   CoordinatesTransformation* CreateCoordinateTransformFromSrs(
                                             const std::string &SpatialReference,
                                             const std::string &Algorithm, int Order);
   /** Metodo auxiliar que a partir de un listado de puntos de control genera la
    *  transformacion de coordenadas correspondiente
    */
   CoordinatesTransformation* CreateCoordinateTransformFromGcp(GcpList* pGcpList,
                                             const std::string &Algorithm, int Order);
   /**
    * Actualiza la transformacion en funcion de los parametros configurados
    * en el part de seleccion de gcps y algoritmo
    */
   bool UpdateCoordinateTransform();
   /**
    * Actualiza la transformacion en funcion de los parametros configurados
    * en el part de seleccion de gcps y algoritmo
    */
   bool UpdateReprojectionParameters();
   /**
    * Genera una lista de puntos de control en funcion de una transformacion exacta.
    */
   void CreateGcpListUsingExact(const ParameterCollection& Params,
                                std::vector<GroundControlPoint>& List);
   /** Part para la configuracion del tipo de reproyeccion */
   WarpParametersPart* pParametersPart_;
   /** Parametros de reproyeccion que utiliza el proceso **/
   ReprojectionProcess::ReprojectionParameters *pReprojectionParameters_;
   /** ultimo path elegido **/
   std::string lastPathChoiced_;
   /** ultimo orden seleccionado **/
   int lastOrderSelected_;
   /** Item de biblioteca de sistema de referencia seleccionado **/
   const LibraryItem* pItemSelected_;
   /** flags para la configuracion del Part **/
   int flags_;
   /** Algoritmo a utilizar por la transformacion */
   std::string algorithm_;
   /** Orden a utilizar por la transformacion */
   int order_;
};
} /** namespace suri */

#endif /* TRANSFORMATIONMODELPARAMETERSPART_H_ */
