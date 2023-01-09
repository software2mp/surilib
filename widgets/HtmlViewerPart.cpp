/*! \file HtmlViewerPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/HtmlViewerPart.h"

// Includes wx
#include <wx/html/htmlwin.h>
#include "wx/xrc/xmlres.h"

// Defines
/** Estructura para el html */
#define HTML_ERROR \
   "																				\
<html><head><title></title></head>															\
	<body bgcolor=\"rgb(150, 150, 150)\">                                      \
		<font face=\"Lucida Grande, Lucida Sans Unicode\">								\
			<table align=\"center\" width=\"460\" bgcolor=\"rgb(255, 255, 255)\" \
												cellspacing=\"0\" cellpadding=\"0\">      \
				<tr>																					\
					<td>																				\
						<table bgcolor=\"rgb(114, 175, 224)\" width=\"100%%\"       \
											cellspacing=\"0\" cellpadding=\"0\">			\
							<tr>																		\
								<td width=\"90%%\" valign=\"center\">						\
									<font	size=\"5\" color=\"rgb(255, 255, 255)\">		\
									<em>%s</em>														\
									</font>															\
								</td>																	\
							</tr>																		\
						</table>																		\
						<table cellpadding=\"10\">												\
							<tr>																		\
								<td align=\"center\">											\
									<h1>																\
										<font color=\"rgb(4, 26, 204)\">%s</font>			\
									</h1>																\
									<em Percentages>%s</em>										\
								</td>																	\
							</tr>																		\
						</table>																		\
					</td>																				\
				</tr>																					\
			</table>																					\
		</font>																						\
	</body>																							\
</html>"

/** inicializa los recursos de la ventana */
void InitGenericHtmlViewerXmlResource();

/** namespace suri */
namespace suri {
/**
 * Ctor. default.
 */
HtmlViewerPart::HtmlViewerPart() {
   windowTitle_ = _(caption_GENERIC_HTML_VIEWER);
   modified_ = false;
   wantedWidth_ = 650;
   wantedHeight_ = 400;
}

/**
 * Ctor.
 * @param[in] Width ancho de la ventana
 * @param[in] Height alto de la ventana
 */
HtmlViewerPart::HtmlViewerPart(int Width, int Height) {
   wantedWidth_ = Width;
   wantedHeight_ = Height;
}

/**
 * Dtor.
 */
HtmlViewerPart::~HtmlViewerPart() {
}
/**
 * Se crea la ventana de la herramienta
 * @return true si pudo crear la ventana de la herramienta, false en otro caso
 */
bool HtmlViewerPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_PANEL_GENHTMLVIEWER"));
   if (!pToolWindow_) {
      return false;
   }
   pToolWindow_->SetMinSize(wxSize(wantedWidth_, wantedHeight_));
   return true;
}

/**
 * Inicializa los controles
 */
void HtmlViewerPart::SetInitialValues() {
   return;
}

/**
 * Objetivo: Setea el nuevo contenido del Html en la ventana.
 * @param[in]	HtmlText: es un html completo.
 * @return true si pudo cargar el html, false si recibe un html vacio
 */
bool HtmlViewerPart::LoadHtml(const std::string &HtmlText) {
   if (HtmlText.empty()) {
      REPORT_AND_FAIL_VALUE("D:Html vacio.", false);
   }

   GET_CONTROL(*pToolWindow_, "ID_HTMLWIN_DESC", wxHtmlWindow)->SetPage(HtmlText);
   return true;
}

/**
 * Objetivo: Carga el url en la ventana.
 * \post si se pudo cargar el html se carga en la ventana
 *	\post si ocurre un error al abrir el archivo se carga Html de error
 * @param[in]	Url: es el url de la pagina a cargar.
 * @return true si pudo cargar el html en la ventana
 * @return false si ocurrio un error al abrir el archivo
 */
bool HtmlViewerPart::LoadUrl(const std::string &Url) {
   wxFileName fn(Url);
   REPORT_DEBUG("D:Archivo: %s", Url.c_str());

   // Si no se creo la ventana devuelve false
   if (!pToolWindow_) {
      return false;
   }

   bool aux = fn.FileExists() &&
   USE_CONTROL(*pToolWindow_, "ID_HTMLWIN_DESC", wxHtmlWindow,
         LoadFile(fn), false);
   if (!aux) {
      wxString htmlerror;
      wxString url(Url);
      url.Replace("/", "/ ", true);
      url.Replace("\\", "\\ ", true);
      htmlerror = wxString::Format(HTML_ERROR, GetWindowTitle().c_str(),
                                   _(message_READ_FILE_ERROR), url.c_str());
      LoadHtml(htmlerror.c_str());
   }
   return aux;
}
}  // namespace suri
