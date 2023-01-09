/*! \file HtmlViewerPart.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef HTMLVIEWERPART_H_
#define HTMLVIEWERPART_H_

// Include suri
#include "suri/Part.h"

/** namespace suri */
namespace suri {
/** Part que se encarga de mostrar html desde un string o URL */
/**
 * \note hereda de Part para permitir cambios en Html
 */
class HtmlViewerPart : public Part {
   /** Ctor. de Copia. */
   HtmlViewerPart(const HtmlViewerPart &HtmlViewerPart);

public:
   /** ctor. */
   HtmlViewerPart();
   /** ctor. */
   HtmlViewerPart(int Width, int Height);
   /** dtor. */
   virtual ~HtmlViewerPart();
   /** Se crea la ventana de herramienta en las clases derivadas. */
   virtual bool CreateToolWindow();
   /** Inicializa los controles. */
   virtual void SetInitialValues();
   /** Devuelve el estado del Part. */
   virtual bool IsEnabled() {
      return true;
   }
   /** Indica si el Part tiene cambios para salvar. */
   virtual bool HasChanged() {
      return modified_;
   }
   /** Salva los cambios realizados en el Part. */
   virtual bool CommitChanges() {
      return true;
   }
   /** Restaura los valores originales del Part. */
   virtual bool RollbackChanges() {
      return true;
   }
   /** Setea el nuevo contenido de la ventana. */
   virtual bool LoadHtml(const std::string &HtmlText);
   /** Carga el url. */
   virtual bool LoadUrl(const std::string &Url);
   /** Actualiza el estado de la parte */
   virtual void Update() {
   }
protected:
private:
};
}  // namespace suri

#endif /*HTMLVIEWERPART_H_*/
