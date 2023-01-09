/*! \file ThresholdClassSelectionPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef THRESHOLDCLASSSELECTIONPART_H_
#define THRESHOLDCLASSSELECTIONPART_H_

// Includes Estandar
#include <vector>

// Includes Suri
#include "ClassificationPart.h"
#include "TablePart.h"
#include "ThresholdClassDefinitionWidget.h"
#include "ThresholdClassificationAlgorithm.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class Element;
class ClassificationAlgorithmInterface;
class Table;
class ThresholdClassEventHandler;

/**
 * Clase encarga de definir las clases involucradas en el proceso de clasificacion
 * supervisada mediante umbrales
 */
class ThresholdClassSelectionPart : public ClassificationPart {
public:
   ThresholdClassSelectionPart(DataViewManager* pDataView, Element* pInputElement);
   virtual ~ThresholdClassSelectionPart();
   /** Retorna las propiedades de los pixels que no tienen clase */
   virtual bool GetNonClassifiedPixelInfo(
                  ClassInformation &Properties) {
      return false;
   }
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update();
   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();
   /** Retorna el nombre del algoritmo que se usara para clasificar */
   virtual std::string GetAlgorithmName();
   /** Retorna un puntero al algoritmo de clasificacion creado por part */
   virtual ClassificationAlgorithmInterface* GetClassificationManager();
   /** Retorna la cantidad de clases */
   virtual int GetClassCount();
   /** Retorna las propiedades de la clase en la posicion indicada. Las Posiciones
    * van de 0 a GetClassCount()-1.**/
   virtual bool GetClassProperties(std::vector<ClassInformation> &Properties);
   /** Retorna el nodo que se debe agregar al elemento que se renderiza */
   virtual wxXmlNode* GetClassificationRendererNode(RasterElement* pRasterElement);
   /**
    * Devuelve el icono de la parte
    * @param[out] ToolBitmap icono de la ventana
    * @param[in] X ancho del icono
    * @param[in] Y alto del icono
    */
   void GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const;

   /** Metodo que indica la seleccion de bandas aciva para la clasificacion **/
   virtual void SetSelectedBands(const std::vector<int>& SelectedBands);

private:
   /**
    *  Metodo que se llama cuando se genera el evento de click sobre el boton
    *  de remover clase
    */
   void OnRemoveClassClick(wxCommandEvent& Event);
   /**
    *  Metodo que se llama cuando se genera el evento de click sobre el boton
    *  de agregar clase
    */
   void OnAddClassClick(wxCommandEvent& Event);
   /**
    * Metodo que se llama cuando se genera el evento de click sobre el boton
    * de editar clase
    */
   void OnEditClassClick(wxCommandEvent& Event);
   /**
    *  Metodo que a partir de las clases definidas en la tabla genera las
    *  clases en una estructura compatible con el algoritmo de clasificacion
    */
   void CreateClassificationClasses(
         std::vector<ThresholdClassificationAlgorithm::ClassThresholdInfo>& Classes);
   /**
    * Metodo auxiliar que a partir de la definicion de umbrales de una clase configura
    * los umbrales en la estructura de clase del algoritmo a utilizar.
    * @param[in] ClassIn definicion de clase de la que se extrae la informacion de umbrales
    * @param[out] Class clase en la que se guarda la configuracion de umbrales
    */
   void LoadClassificationBandInfo(
         ThresholdClassDefinitionWidget::ThresholdClassInformation& ClassIn,
         ThresholdClassificationAlgorithm::ClassThresholdInfo& Class);
   /** Metodo auxiliar que configura la tabla para la generacion de clases por umbrales **/
   void ConfigureClassTable();
   /**
    *  Metodo que actualiza la definicion de las clases en funcion de lo que se
    *  encuentra configurado en la tabla
    */
   void LoadClassesFromTable();
   friend class ThresholdClassEventHandler;
   ThresholdClassEventHandler* pEventHandler_;
   Element* pInputElement_;
   Table* pTable_;
   TablePart* pTablePart_;
   DataViewManager* pDataView_;
   typedef std::vector<ThresholdClassDefinitionWidget::ThresholdClassInformation> ClassVector;
   ClassVector classes_;
   ThresholdClassificationAlgorithm* palgorithm_;
   /** vector con la seleccion de bandas **/
   std::vector<int> selectedBands_;
};
} /** namespace suri */

#endif /* THRESHOLDCLASSSELECTIONPART_H_ */
