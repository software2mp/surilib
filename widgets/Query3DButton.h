/*! \file Query3DButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef QUERY3DBUTTON_H_
#define QUERY3DBUTTON_H_

// Includes suri
#include "QueryButton.h"
#include "suri/Dimension.h"
// Includes ogr

/** Define la cantidad de puntos a utilizar para la reproyeccion del querybox */
#define _VECTORQUERY_EXTENT_POINTS_ 100

/** namespace suri */
namespace suri {
/** QueryButton que abre archivo asociado cuando se hace click sobre un link en vista 3D */
class Query3DButton : public QueryButton {
   /** Ctor. de Copia. */
   Query3DButton(const Query3DButton &Query3DButton);

public:

   /** Ctor */
   /**
    * @param[in] pToolbar window donde se debe agregar el boton
    * @param[in] pGlCanvas canvas 3D que se usa para saber sobre que vector se
    * hizo click.
    * @param[in] pViewer viewer donde se esperan clicks para activar hotlink.
    */
   Query3DButton(wxWindow *pToolbar, TerrainGLCanvas *pGlCanvas, ViewerWidget *pViewer) :
         QueryButton(pToolbar, pViewer), pGlCanvas_(pGlCanvas) {
   }
   /** Dtor */
   virtual ~Query3DButton() {
   }

protected:

   /** Activa accion TerrainGLCanvas::Query en GlCanvas */
   /**
    * @param[in] Event evento de click
    */
   virtual void OnButtonClicked(wxCommandEvent &Event) {
      SetValue(GetValue());
      pGlCanvas_->SetAction(TerrainGLCanvas::Query);
   }

   /** Analiza si el click puede pertenecer a un elemento valido. */
   /**
    * @param[in] PosX posicion(X) sobre el que se hizo click
    * @param[in] PosY posicion(Y) sobre el que se hizo click
    */
   virtual bool IsValid(int PosX, int PosY) {
      return pGlCanvas_->Selection(PosX, PosY);
   }

   /** Retorna un box con el tamanio del elemento centrado en click. */
   /**
    * El subset retornado esta en coordenadas de la imagen.
    * @param[in] PosX lugar donde se hizo click x
    * @param[in] PosY lugar donde se hizo click y
    * @param[in] ElementSize tamanio del elemento
    * @param[out] QueryBox box con dimencion del elemento centrado en click
    * y en coordenadas de la imange.
    */
   bool GetQueryBox(int PosX, int PosY, int ElementSize, Subset &QueryBox) {
      Coordinates clickposition = pGlCanvas_->GetPosition(PosX, PosY);

      double radius = ElementSize * SPHERE_RADIUS
            / pGlCanvas_->GetTerrain()->GetPixelSize();

      // A pesar que las coordenadas no son ul y lr en el sistema del terreno
      // si lo son en sistema de viewport. Por lo que no sera necesario
      // reacomodarlos luego de llamar a GetRasterPosition
      radius = radius * 1.1;
      QueryBox.ul_.x_ = clickposition.x_ - radius;
      QueryBox.ul_.y_ = clickposition.y_ - radius;
      QueryBox.lr_.x_ = clickposition.x_ + radius;
      QueryBox.lr_.y_ = clickposition.y_ + radius;

      QueryBox.ul_ = pGlCanvas_->GetTerrain()->GetRasterPosition(QueryBox.ul_);
      QueryBox.lr_ = pGlCanvas_->GetTerrain()->GetRasterPosition(QueryBox.lr_);

      return true;
   }

private:
   TerrainGLCanvas *pGlCanvas_; /*! GlCanvas */
};
}

#endif /*QUERY3DBUTTON_H_*/
