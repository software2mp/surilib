/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

#include "ZoomImageScaleButton.h"

// Includes standard

// Includes suri
#include "suri/Viewer2D.h"
#include "suri/messages.h"
#include "resources.h"
#include "suri/RasterSpatialModel.h"
#include "suri/World.h"
#include "suri/Dimension.h"
#include "logmacros.h"
#include "suri/Element.h"
#include "suri/LayerList.h"
#include "suri/RasterElement.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/**
 * Busca en la lista un RasterElement activo, si lo encuentra calcula el
 * window y su posicion para que la relacion entre el pixel de la imagen y
 * la pantalla sea 1:1.
 * @param[in] Event evento generado hacer click con el mouse
 */
void ZoomImageScaleButton::OnButtonClicked(wxCommandEvent &Event) {
   RasterElement *pelement = NULL;
   // Recorro la lista buscando un Raster activo
   if (!pList_ || pList_->GetActiveCount() == 0) {
      return;
   }

   LayerList::ElementListType::iterator it = pList_->GetElementIterator();
   while (it != pList_->GetElementIterator(false)) {
      if ((*it)->IsActive() && dynamic_cast<suri::RasterElement*>(*it)) {
         pelement = dynamic_cast<suri::RasterElement*>(*it);
         break;
      }
      it++;
   }
   if (!pelement) {
      REPORT_DEBUG("D:No se pudo encontrar el raster activo");
      return;
   }

   // Obtengo el RasterSpatialModel y calculo el tamano del window para pixel 1:1
   RasterSpatialModel *prastermodel = RasterSpatialModel::Create(
         pelement->GetRasterModel());
   if (!prastermodel) {
      return;
   }

   Subset newwindow(0, 0, 0, 0);
   int x, y;
   pViewer_->GetWorld()->GetViewport(x, y);
   newwindow.lr_.x_ = x;
   newwindow.lr_.y_ = y;
   prastermodel->Transform(newwindow.lr_);
   prastermodel->Transform(newwindow.ul_);
   RasterSpatialModel::Destroy(prastermodel);
   // encuentro la posicion final de la ventana
   Subset oldwindow;
   pViewer_->GetWorld()->GetWindow(oldwindow);
   double oldcenter_x = (oldwindow.lr_.x_ + oldwindow.ul_.x_) / 2;
   double oldcenter_y = (oldwindow.lr_.y_ + oldwindow.ul_.y_) / 2;
   double difx = (newwindow.lr_.x_ - newwindow.ul_.x_) / 2;
   double dify = (newwindow.lr_.y_ - newwindow.ul_.y_) / 2;
   newwindow.lr_.x_ = oldcenter_x + difx;
   newwindow.ul_.x_ = oldcenter_x - difx;
   newwindow.lr_.y_ = oldcenter_y + dify;
   newwindow.ul_.y_ = oldcenter_y - dify;
   pViewer_->ChangeWindow(newwindow);
}
}
