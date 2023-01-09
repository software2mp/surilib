/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "ColorTableRendererTest.h"

// Includes standard
#include <sstream>
// Includes Suri
#include "ColorTableRenderer.h"
#include "suri/RasterElement.h"
#include "suri/Configuration.h"
#include "suri/xmlnames.h"
#include "RasterRenderer.h"
#include "suri/World.h"
#include "suri/Subset.h"
#include "MemoryCanvas.h"
#include "RenderPipeline.h"
#include "Mask.h"
// Includes Wx
#include "wx/xml/xml.h"
// Includes App
// Defines

/** namespace suri */
namespace suri {
/** Ctor */
ColorTableRendererTest::ColorTableRendererTest() {
}

/**
 * Test de crear renderizador a partir de elemento.
 * Falla si no se pudo crear renderizador
 */
void ColorTableRendererTest::TestCreateOk() {
   // Creo parameters con dato nodo
   std::string pathdir = suri::Configuration::GetParameter("app_base_dir_volatile", "");
   pathdir += "../lib/suri/test/imagenes/red.bmp";
   suri::RasterElement* pre = suri::RasterElement::Create(pathdir);
   wxXmlNode* renderernode = pre->GetNode(wxT(RENDERIZATION_NODE));

   pathdir = suri::Configuration::GetParameter("app_base_dir_volatile", "");
   pathdir += "../lib/suri/test/imagenes/colortable.xml";

   wxXmlDocument ctdocument(pathdir);
   wxXmlNode* pctnode = ctdocument.GetRoot();

   wxString aux = wxString(wxT(RENDERIZATION_NODE)) + wxT("|")
         + wxString(wxT(LUT_NODE));
   wxXmlNode* poldlutnode = pre->GetNode(aux.c_str());
   renderernode->InsertChildAfter(pctnode, poldlutnode);
   renderernode->RemoveChild(poldlutnode);
   delete poldlutnode;

   suri::RasterRenderer rasterrendaux;
   suri::RasterRenderer* prasterrend = rasterrendaux.Create(pre, NULL);

   suri::ColorTableRenderer ctaux;
   suri::ColorTableRenderer* pcolortablerenderer =
         (suri::ColorTableRenderer*) ctaux.Create(pre, prasterrend);

   CPPUNIT_ASSERT_MESSAGE("Error al crear lut", pcolortablerenderer!=NULL);
}

/**
 * Falla si crea renderizador a pesar que en elemento hay nodo LUT
 * en vez de colortable.
 */
void ColorTableRendererTest::TestCreateError() {
   // Creo parameters con dato nodo
   std::string pathdir = suri::Configuration::GetParameter("app_base_dir_volatile", "");
   pathdir += "../lib/suri/test/imagenes/red.bmp";
   suri::RasterElement* pre = suri::RasterElement::Create(pathdir);
   wxXmlNode* renderernode = pre->GetNode(wxT(RENDERIZATION_NODE));

   pathdir = suri::Configuration::GetParameter("app_base_dir_volatile", "");
   pathdir += "../lib/suri/test/imagenes/lut.xml";

   wxXmlDocument ctdocument(pathdir);
   wxXmlNode* pctnode = ctdocument.GetRoot();

   wxString aux = wxString(wxT(RENDERIZATION_NODE)) + wxT("|")
         + wxString(wxT(LUT_NODE));
   wxXmlNode* poldlutnode = pre->GetNode(aux.c_str());
   renderernode->InsertChildAfter(pctnode, poldlutnode);
   renderernode->RemoveChild(poldlutnode);
   delete poldlutnode;

   suri::RasterRenderer rasterrendaux;
   suri::RasterRenderer* prasterrend = rasterrendaux.Create(pre, NULL);

   suri::ColorTableRenderer ctaux;
   suri::ColorTableRenderer* pcolortablerenderer =
         (suri::ColorTableRenderer*) ctaux.Create(pre, prasterrend);

   CPPUNIT_ASSERT_MESSAGE("No detecto error al crear lut", pcolortablerenderer==NULL);
}

/**
 * Con xml leido de disco crea parametros. Utiliza los parametros para
 * crear otro nodo y los compara
 * Falla si no son iguales
 */
void ColorTableRendererTest::TestGetXmlNode() {
   // Creo parameters con dato nodo
   std::string pathdir = suri::Configuration::GetParameter("app_base_dir_volatile", "");
   pathdir += "../lib/suri/test/imagenes/red.bmp";
   suri::RasterElement* pre = suri::RasterElement::Create(pathdir);
   wxXmlNode* prenderernode = pre->GetNode(wxT(RENDERIZATION_NODE));

   if (!prenderernode) {
      REPORT_DEBUG("D:FALLO TEST");
      delete pre;
      return;
   }

   pathdir = suri::Configuration::GetParameter("app_base_dir_volatile", "");
   pathdir += "../lib/suri/test/imagenes/colortable.xml";

   wxXmlDocument ctdocument(pathdir);
   wxXmlNode* pctnode = ctdocument.GetRoot();

   wxString aux = wxString(wxT(RENDERIZATION_NODE)) + wxT("|")
         + wxString(wxT(LUT_NODE));
   wxXmlNode* poldlutnode = pre->GetNode(aux.c_str());
   prenderernode->InsertChildAfter(pctnode, poldlutnode);
   prenderernode->RemoveChild(poldlutnode);
   delete poldlutnode;

   std::string oldxml = pre->GetXmlAsString().c_str();
   REPORT_DEBUG(pre->GetXmlAsString().c_str());

   suri::ColorTableRenderer::Parameters params =
         suri::ColorTableRenderer::GetParameters(pre->GetNode(wxT("")));

   // Creo nodo con dato parameters y lo remplazo en elemento y comparo
   wxXmlNode* pnewctnode = suri::ColorTableRenderer::GetXmlNode(params);
   prenderernode->InsertChildAfter(pnewctnode, pctnode);
   prenderernode->RemoveChild(pctnode);

   REPORT_DEBUG(oldxml.c_str());
   REPORT_DEBUG(pre->GetXmlAsString().c_str());

   CPPUNIT_ASSERT_MESSAGE("ERROR en xml nuevo",
                          oldxml.compare(pre->GetXmlAsString() )==0);
}
/**
 * Test de crear objeto Parameters
 * Falla si parameters tiene banda origen incorrecta o primer tabla de
 * esta mal
 */
void ColorTableRendererTest::TestGetParameters() {
   std::string pathdir = suri::Configuration::GetParameter("app_base_dir_volatile", "");
   pathdir += "../lib/suri/test/imagenes/red.bmp";
   suri::RasterElement* pre = suri::RasterElement::Create(pathdir);
   wxXmlNode* prenderernode = pre->GetNode(wxT(RENDERIZATION_NODE));

   if (!prenderernode) {
      delete pre;
      REPORT_DEBUG("D:FALLO TEST");
      return;
   }

   pathdir = suri::Configuration::GetParameter("app_base_dir_volatile", "");
   pathdir += "../lib/suri/test/imagenes/colortable.xml";

   wxXmlDocument ctdocument(pathdir);
   wxXmlNode* pctnode = ctdocument.GetRoot();

   wxString aux = wxString(wxT(RENDERIZATION_NODE)) + wxT("|")
         + wxString(wxT(LUT_NODE));
   wxXmlNode* poldlutnode = pre->GetNode(aux.c_str());
   prenderernode->InsertChildAfter(pctnode, poldlutnode);
   prenderernode->RemoveChild(poldlutnode);
   delete poldlutnode;

   REPORT_DEBUG(pre->GetXmlAsString().c_str());

   suri::ColorTableRenderer::Parameters params =
         suri::ColorTableRenderer::GetParameters(pre->GetNode(wxT("")));

   // Verifico que tabla para banda 0 sea correcta
   std::stringstream keysstream;
   std::stringstream valuesstream;

   std::map<double, double> table;
   params.colorTable_.GetLookUpTable(0).GetTable(table);
   std::map<double, double>::const_iterator it = table.begin();
   while (it != table.end()) {
      keysstream << (*it).first;
      valuesstream << (*it).second;
      valuesstream << " ";
      keysstream << " ";
      it++;
   }

   CPPUNIT_ASSERT_MESSAGE(
         "ERROR en datos de tabla ",
         ( (keysstream.str().compare("3 15 600 1.79769e+308 ")==0) || (valuesstream.str().compare("1 2 3 3 ")==0) ));
}

/**
 * Test de renderizado
 * Renderiza imagen de 4 pixel y le aplica mapa. Si el resultado
 * tiene valores incorrectos falla.
 */
void ColorTableRendererTest::TestRender() {
   // Creo parameters con dato nodo
   std::string pathdir = suri::Configuration::GetParameter(
         "app_base_dir_volatile", "");
   pathdir += "../lib/suri/test/imagenes/red.bmp";
   suri::RasterElement* pre = suri::RasterElement::Create(pathdir);
   wxXmlNode* prenderernode = pre->GetNode(wxT(RENDERIZATION_NODE));

   pathdir = suri::Configuration::GetParameter("app_base_dir_volatile", "");
   pathdir += "../lib/suri/test/imagenes/colortable.xml";

   wxXmlDocument ctdocument(pathdir);
   wxXmlNode* pctnode = ctdocument.GetRoot();

   wxString aux = wxString(wxT(RENDERIZATION_NODE)) + wxT("|")
         + wxString(wxT(LUT_NODE));
   wxXmlNode* poldlutnode = pre->GetNode(aux.c_str());
   prenderernode->InsertChildAfter(pctnode, poldlutnode);
   prenderernode->RemoveChild(poldlutnode);
   delete poldlutnode;

   int width, height = 0;
   suri::Subset worldextent;
   pre->GetElementExtent(worldextent);
   pre->GetRasterSize(width, height);
   suri::World world;
   world.SetSpatialReference(pre->GetSpatialReference().c_str());
   world.SetViewport(width, height);
   world.SetWorld(worldextent);
   world.SetWindow(worldextent);

   suri::Mask *pmask = NULL;
   suri::RenderPipeline* ppipeline = suri::RenderPipeline::Create(pre);
   suri::Canvas* pmc = new suri::MemoryCanvas();
   ppipeline->Render(&world, pmc, pmask);

   // Leo los datos en el canvas
   std::vector<int> bands(3);
   std::vector<void*> data(3);
   for (int b = 0; b < 3; b++) {
      bands[b] = b;
      data[b] = new unsigned char[width * height * pmc->GetDataSize()];
   }
   pmc->Read(bands, data);
   std::string auxs = pmc->GetDataType();
   // Defino streams para cargar datos
   std::stringstream valuesstream;
   // Recorro map y cargo nodos(no cargo el ultimo)
   for (int i = 0; i < 3; i++) {
      unsigned char* datachar = (unsigned char*) data[i];
      for (int j = 0; j < 4; j++) {
         valuesstream << wxString::Format("%d", datachar[j]).c_str() << " ";
      }
   }REPORT_DEBUG(valuesstream.str().c_str());

   CPPUNIT_ASSERT_MESSAGE("ERROR en render",
                          valuesstream.str()=="2 3 1 1 4 6 4 4 9 9 7 7 ");
}

/**
 * Test de renderizado
 * Renderiza imagen de 4 pixel y le aplica mapa. Si el resultado
 * tiene valores incorrectos falla.
 */
void ColorTableRendererTest::TestRenderWithDataCast() {
   // Creo parameters con dato nodo
   std::string pathdir = suri::Configuration::GetParameter(
         "app_base_dir_volatile", "");
   pathdir += "../lib/suri/test/imagenes/imagengris.tif";
   suri::RasterElement* pre = suri::RasterElement::Create(pathdir);
   wxXmlNode* prenderernode = pre->GetNode(wxT(RENDERIZATION_NODE));

   pathdir = suri::Configuration::GetParameter("app_base_dir_volatile", "");
   pathdir += "../lib/suri/test/imagenes/colortable.xml";

   wxXmlDocument ctdocument(pathdir);
   wxXmlNode* pctnode = ctdocument.GetRoot();

   wxString aux = wxString(wxT(RENDERIZATION_NODE)) + wxT("|")
         + wxString(wxT(LUT_NODE));
   wxXmlNode* poldlutnode = pre->GetNode(aux.c_str());
   prenderernode->InsertChildAfter(pctnode, poldlutnode);
   prenderernode->RemoveChild(poldlutnode);
   delete poldlutnode;

   REPORT_DEBUG(pre->GetXmlAsString().c_str());

   int width, height = 0;
   suri::Subset worldextent;
   pre->GetElementExtent(worldextent);
   pre->GetRasterSize(width, height);
   suri::World world;
   world.SetSpatialReference(pre->GetSpatialReference().c_str());
   world.SetViewport(width, height);
   world.SetWorld(worldextent);
   world.SetWindow(worldextent);

   suri::Mask *pmask = NULL;
   suri::RenderPipeline* ppipeline = suri::RenderPipeline::Create(pre);
   suri::Canvas* pmc = new suri::MemoryCanvas();
   ppipeline->Render(&world, pmc, pmask);

   // Leo los datos en el canvas
   std::vector<int> bands(3);
   std::vector<void*> data(3);
   for (int b = 0; b < 3; b++) {
      bands[b] = b;
      data[b] = new unsigned char[width * height * pmc->GetDataSize()];
   }
   pmc->Read(bands, data);
   std::string auxs = pmc->GetDataType();
   // Defino streams para cargar datos
   std::stringstream valuesstream;
   // Recorro map y cargo nodos(no cargo el ultimo)
   for (int i = 0; i < 3; i++) {
      unsigned char* datachar = (unsigned char*) data[i];
      for (int j = 0; j < 4; j++) {
         valuesstream << wxString::Format("%d", datachar[j]).c_str() << " ";
      }
   }REPORT_DEBUG(valuesstream.str().c_str());

   CPPUNIT_ASSERT_MESSAGE("ERROR en render",
                          valuesstream.str()=="1 1 1 1 4 4 4 4 7 7 7 7 ");
}
}  // namespace suri

