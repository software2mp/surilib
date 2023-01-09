/*! \file SpectralAngleMapperPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SPECTRALANGLEMAPPERPART_H_
#define SPECTRALANGLEMAPPERPART_H_

// Includes Estandar
// Includes Suri
#include "ClassificationPart.h"
#include "suri/Part.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/LibraryItemSelectionInterface.h"
#include "SpectralAngleMapperAlgorithm.h"
// Includes Wx
// Defines

namespace suri {

// forwards
class BasicLibraryItemSelector;
class LibraryClient;
class LibraryManager;
class Table;
class TablePart;

/** Declara clase MergeLayersPartEvent */
DECLARE_EVENT_OBJECT(SpectralAngleMapperPartEvent);

/**
 * Control grafico para el mapeo de angulos espectrales
 */
class SpectralAngleMapperPart : public ClassificationPart, public LibraryItemSelectionInterface {
   FRIEND_EVENT_OBJECT(SpectralAngleMapperPartEvent);
public:
   /** Constructor */
   SpectralAngleMapperPart(LibraryManager* pManager);
   /** Destructor */
   virtual ~SpectralAngleMapperPart();
   /** Retorna las propiedades de los pixels que no tienen clase */
   virtual bool GetNonClassifiedPixelInfo(
                  ClassInformation &Properties) {
      return false;
   }
   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged();
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges();
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges();
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
   /** Metodo que indica la seleccion de bandas aciva para la clasificacion **/
   virtual void SetSelectedBands(const std::vector<int>& SelectedBands);
   /** Retorna el icono de la herramienta */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;

   /** Metodo que sirve para verificar si la clasificacion es valida */
   virtual bool IsValid();

private:
   /**
    *  Metodo que a partir de las clases definidas en la tabla genera las
    *  clases en una estructura compatible con el algoritmo de clasificacion
    */
   void CreateClassificationClasses(
         std::vector<SpectralAngleMapperAlgorithm::ClassSpectralInfo>& Classes);
   /** Agrega una clase a la tabla */
   void OnAddClassClick(wxCommandEvent &Event);
   /** Elimina una clase de la tabla */
   void OnDeleteClassClick(wxCommandEvent &Event);
   /** Accion de ingresar un valor en el TextCtrl del angulo maximo */
   void OnSamTexCtrlChanged(wxCommandEvent &Event);
   /** Objeto que conecta los eventos */
   EVENT_OBJECT_PROPERTY(SpectralAngleMapperPartEvent);
   /** Inicializa el part */
   virtual void SetInitialValues();
   /** Actualiza el estado de la parte */
   virtual void Update() {};
   /** Devuelve si la parte contiene datos validos. */
   virtual bool HasValidData();
   void SetActiveItem(const LibraryItem* pItem){};
   /** Configura la tabla */
   void ConfigureFeatures();
   /** Agrega las columnas de la tabla */
   void AppendColumns();
   /** Verifica que la clase que se quiere agregar no este duplicada */
   bool CanAddClass(std::string ItemValue);
   /**
    * Metodo auxiliar que a partir de la definicion de umbrales de una clase configura
    * los umbrales en la estructura de clase del algoritmo a utilizar.
    * @param[in] ClassIn definicion de clase de la que se extrae la informacion de umbrales
    * @param[out] Class clase en la que se guarda la configuracion de umbrales
    */
   void LoadClassificationBandInfo(SpectralAngleMapperAlgorithm::ClassSpectralInfo& Class,
                                   std::string Principal);


   /**
    * Valida que la cantidad de registros en la firma se la misma
    * que bandas en el raster seleccionado.
    */
   bool HasValidBandsAndClasses() const;

   /** Devuelve si la parte contiene datos validos */
   bool hasvaliddata_;
   /** Selector de biblioteca con firmas espectrales */
   BasicLibraryItemSelector* pItemSelector_;
   /** Cliente de biblioteca */
   LibraryClient* pClient_;
   /** Definicion de columna Clase */
   static const int CLASS_COL = 0;
   /** Definicion de columna Color */
   static const int HEXCOLOR_COL = 1;
   /** Definicion de nombre de columna Clase */
   static const std::string Clase;
   /** Definicion de nombre de columna Color */
   static const std::string Color;
   /** Tabla que contiene las clases */
   Table* pTable_;
   TablePart* pTablePart_;
   SpectralAngleMapperAlgorithm* palgorithm_;
   /** vector con la seleccion de bandas **/
   std::vector<int> selectedBands_;
};

} /** namespace suri */

#endif /* SPECTRALANGLEMAPPERPART_H_ */
