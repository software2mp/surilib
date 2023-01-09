/** \file Viewer2DSelectorPart.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWER2DSELECTOR_H_
#define VIEWER2DSELECTOR_H_

#include <map>
#include <string>
#include <sstream>
#include <vector>

#include "suri/Part.h"
#include "suri/SuriObject.h"

namespace suri {

DECLARE_EVENT_OBJECT(Viewer2DSelectorPartEvent);

class Viewer2DSelectorPart : public Part {
public:
   FRIEND_EVENT_OBJECT(Viewer2DSelectorPartEvent);

   static const std::string kViewer;
   static const int kInitialBasePos = 0;
   static const int kInitialWarpPos = 1;


   /**
    * Ctor
    */
   Viewer2DSelectorPart(bool Enable = true, bool Modified = false);

   /**
    * Dtor
    */
   virtual ~Viewer2DSelectorPart();

   /**
    * Indica si el Part tiene cambios para salvar
    */
   virtual bool HasChanged();

   /**
    * Salva los cambios realizados en el Part.
    */
   virtual bool CommitChanges();

   /**
    * Restaura los valor's originales del Part.
    */
   virtual bool RollbackChanges();

   /**
    * Inicializa el part
    */
   virtual void SetInitialValues();

   /**
    * Actualiza el estado de la parte
    */
   virtual void Update();

   /**
    * Modifica el nombre del viewer.
    */
   virtual bool ConfigureWidget();

   /**
    * Devuelve si la parte tiene datos validos
    */
   virtual bool HasValidData();

   /**
    * Configura los visualizadores disponibles.
    */
   void SetCurrentViewers(const std::map<SuriObject::UuidType, std::string>& CurrentViewers);

   /**
    * Obtiene los visualizadores seleccionados.
    */
   std::vector<SuriObject::UuidType> GetSelectedViewers() const;

   /**
    * Handler para el evento de cambio de seleccion de choice base.
    */
   virtual void OnChBaseSelectionChanged(wxCommandEvent &Event);

   /**
    * Handler para el evento de cambio de seleccion de choice warp.
    */
   virtual void OnChWarpSelectionChanged(wxCommandEvent &Event);

protected:
   EVENT_OBJECT_PROPERTY(Viewer2DSelectorPartEvent);

   int baseSelection_;
   int warpSelection_;
   SuriObject::UuidType baseId_;
   SuriObject::UuidType warpId_;

   std::map<SuriObject::UuidType, std::string> currentViewers_;

   /**
    * Inicializa el part realizando todas las configuraciones necesarias.
    */
   void InitializePart();

   /**
    * Inicializa los controles choice para la seleccion de visualizadores.
    */
   void InitializeChoices();

   /**
    * Inicializa el control choice para visualizador base.
    */
   void InitializeChoiceBase();

   /**
    * Inicializa el control choice para visualizador warp.
    */
   void InitializeChoiceWarp();

   /**
    * Inicializacion general.
    */
   virtual void Initialize();

   /**
    * Verifica si los visualizadores seleccionados son distintos. De no serlos
    * muestra un mensaje al usuario.
    */
   bool AreEqualChoices() const;
   /**
    * Redibujo panel padre
    */
   void RefreshContainer();
};

}  // namespace suri

#endif  // VIEWER2DSELECTOR_H_
