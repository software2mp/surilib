/*! \file Part.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PART_H_
#define PART_H_

// Includes suri
#include "Widget.h"

// Includes wx
#include "wx/string.h"

// defines
#ifdef __WXMSW__
/** Ancho minimo de los Part */
#define MINIMUM_PART_WIDTH 380
/** Alto minimo de los Part */
#define MINIMUM_PART_HEIGHT 350
#else
/** Ancho minimo de los Part */
#define MINIMUM_PART_WIDTH 545
/** Alto minimo de los Part */
#define MINIMUM_PART_HEIGHT 355
#endif

/** namespace suri */
namespace suri {
/*! \page ProcessPagePart Generar pagina de proceso
 Version 0.2
 \section sec1 Descripcion

 \subsection subsection1 Caracteristicas de las paginas
 - Define interfaz grafica con la que interactua el usuario. Tambien responde a
 eventos relacionados con logica interna de pagina.
 - Tiene un  proceso asociado:
 - Se encarga de validar datos ingresados por usuario.
 - Tiene parametros de configuracion iniciales.
 - Implementa logica como si fuera una transaccion, se deben implementar
 metodos Commit (actualiza el proceso con la interfaz) y Rollback(actualiza
 interfaz con datos en proceso).

 \subsection subsection2 Modo operacion
 -# Al construirse recibe el proceso a configurar.
 -# Presenta interfaz al usuario para que ingrese parametros de configuracion
 de un proceso
 -# Los campos se inicializan con los valores default en objeto de proceso.
 (SetInitialValues).
 -# Al interactuar con el usuario los valores ingresados se mantienen en
 interfaz grafica o variables internas, pero nunca se modifica el proceso.
 -# Si el usuario aplica le pide a proceso que valide los datos(Commit)
 - Si los datos estan bien configurados commit tiene exito y el proceso
 queda configurado con los nuevos parametros
 - Caso contrario se informa al usuario. El usuario puede hacer rollback o
 corregir la configuracion
 -# Si el usuario cancela se hace rollback.

 \subsection subsection3 Interaccion con procesos
 La pagina solo utiliza las interfaces del proceso relacionadas con los
 parametros que quiere configurar, esto permite utilizar la misma pagina
 para configurar varios procesos.

 La interfaz tiene Seters para parametros y un metodo validate, la
 implementacion de la interfaz se encarga de realizar la validacion que
 corresponda.

 \subsection subsection4 Interaccion con capas
 Las capas y otros objetos core que requiera la pagina se reciben en ctor.
 Estos objetos se pueden usar para configurar los procesos, ya se obteniendo
 algun dato de ellos o usando el objeto mismo.

 Las capas no deben ser modificadas por el Part ya que son una entrada al proceso.

 \section sec2 Pasos

 \subsection subsection5 Crear clase que herede ProcessConfigurationPart

 \subsubsection subsubsection1 Interfaz a definir

 Ctor
 -# Crea, configura y guarda puntero a los subparts que requiera
 -# Agrega subparts con AddPart
 .
 Dtor.
 -# Setea punteros de subparts a NULL ya que seran destruidos cuando se llame
 a dtor de Part
 HasChanged:
 -# Llama a SubpartsHasChanged() o para cada subpart que corresponda llama a
 HasChanged. Si la respuesta de alguno es true retorna true.
 -# Obtiene clases de proceso usando IProcessConfiguration
 -# Compara los valores en interfaz contra los del proceso, si alguno es
 diferente retorna true.
 .
 HasValidData:
 -# Llama a SubpartsHasValidData() o para cada subpart que corresponda llama
 a HasValidData. Si la respuesta es true retorna true.
 -# Obtiene clases de proceso usando IProcessConfiguration
 -# Compara los valores en interfaz contra los del proceso, si alguno es
 diferente retorna true.
 .
 CommitChanges:
 -# Llama a HasValidData, si retorna false retorna false.
 -# Llama a SubpartsCommitChanges() o para cada subpart que corresponda
 llama a CommitChanges. Si la respuesta de alguno es false retorna false.
 -# Inicializa con IProcessConfiguration las clases a configurar
 -# Carga los valores ingresados por el usuario en las clases
 -# Actualiza IProcessConfiguration con las clases configuradas. Si falla le
 informa al usuario que hubo un error en la configuracion.
 .
 RollbackChanges:
 -# Llama a SubpartsRollbackChanges() o para cada subpart que corresponda
 llama a RollbackChange.
 -# Obtiene clases de proceso usando IProcessConfiguration
 -# Actualiza interfaz con los valores en clases
 .

 \subsubsection subsubsection2 Template codigo

 - \<NombreClase\>Part.h

 \code
 class <NombreClase>Part: public ProcessConfigurationPart
 {
 public:
 <NombreClase>Part(IProcessConfiguration pProcessConfiguration
 <, Objetos core requeridos>);
 virtual ~<NombreClase>Part();

 //! Indica si el Part tiene cambios para salvar.
 virtual bool HasChanged();
 //! Devuelve si la parte tiene datos validos
 virtual bool HasValidData();
 //! Salva los cambios realizados en el Part.
 virtual bool CommitChanges();
 //! Restaura los valores originales del Part.
 virtual bool RollbackChanges();
 };
 \endcode

 - \<NombreClase\>Part.cpp

 \code
 //!Ctor
 <NombreClase>Part::<NombreClase>Part(IProcessConfiguration pProcessConfiguration
 <, Objetos core requeridos>): ProcessConfigurationPart (pProcessConfiguration)
 {
 < #codigo Configurar xrc >
 < #codigo Guardar objetos core >
 }

 //!Dtor
 <NombreClase>Part::~<NombreClase>Part()
 {
 < #codigo Limpio punteros a objetos core >
 }

 //! Indica si el Part tiene cambios para salvar.
 bool <NombreClase>Part::HasChanged()
 {
 If(SubpartsHasChanged()) return true;

 < #codigo Comparar datos en interfaz contra datos en proceso >

 }

 //! Devuelve si la parte tiene datos validos
 bool <NombreClase>Part::HasValidData()
 {
 If(!SubpartsHasValidData()) return false;

 < #codigo Obtener objetos 'IProcess..' para validar >

 }

 //! Salva los cambios realizados en el Part.
 bool <NombreClase>Part::CommitChanges()
 {
 If(!HasValidData()) return false;

 If(!SubpartsCommitChanges ()) return false;

 < #codigo Configurar proceso con datos en interfaz >
 }

 //! Restaura los valores originales del Part.
 bool <NombreClase>Part::RollbackChanges()
 {
 If(!SubpartsRollbackChanges()) return false;

 < #codigo Configurar interfaz con datos en proceso >
 }

 \endcode

 - Ejemplo obtener objeto a configurar

 \code
 \endcode

 */

/** Widget abstracto que declara funcionalidad basica para pantallas de datos */
/**
 * Declara metodos para consultar estado de la parte, hacer Commit/Rollback
 * y ver si ocurrieron modificaciones
 * \note extiende la interfaz de Widget con funcionalidad para transacciones
 */
class Part : public Widget {
   /** Ctor. de Copia */
   Part(const Part &Part);
public:
   /** Ctor. */
   Part(const bool &Enable = true, const bool &Modified = false);
   /** Ctor. */
   Part(Widget *pWidget, const bool &Enable = true, const bool &Modified = false);
   /** Ctor que genera pWidget_ usando XrcId. */
   Part(const std::string &XrcId, const std::string &PartName = "", const bool &Enable =
         true,
        const bool &Modified = false);
   /** Dtor. */
   virtual ~Part();
   /** Setea el padre del Part y llama a CreateTool. */
   virtual bool CreatePartTool(wxWindow *pParentWindow);
   /** Devuelve el estado del Part. */
   virtual bool IsEnabled();
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged()=0;
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges()=0;
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges()=0;
   /** Inicializa el part */
   virtual void SetInitialValues()=0;
   /** Cambia el estado de enabled_ */
   virtual void SetEnabled(bool Enabled);
   /** Devuelve si la parte tiene datos validos */
   virtual bool HasValidData();
   /** Actualiza el estado de la parte */
   virtual void Update()=0;
   /** Operador == */
   virtual bool operator==(const Part &Other);

   /** Retorna el Widget con la interfaz que usa el part */
   Widget *GetWidget();
   /** Permite a clases que hereden modificar el widget luego de crear la ventana */
   virtual bool ConfigureWidget() {
      return true;
   }
   /** Implementacion del metodo de creacion de ventana de Widget */
   virtual bool CreateToolWindow();
protected:
   bool enabled_; /*! indica si el Part esta o no habilitado. */
   bool modified_; /*! indica si el Part esta o no modificado. */
   wxString errorMessage_; /*! Mensaje de error que se carga en HasValidData */
   Widget *pWidget_; /*! Puntero al Widget con la interfaz que usa el part. */
private:
};
}  // namespace suri

#endif /*PART_H_*/
