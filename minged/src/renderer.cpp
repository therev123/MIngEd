#include "renderer.h"
#include "atlas.h"

#include "MCore.h"
#include "MEngine.h"

#define MAX_QUAD 64

extern minged::Atlas* g_Selected;

namespace minged
{
namespace Renderer
{
    MVector3 g_Vertices[MAX_QUAD * 6];
    MVector2 g_UVs[MAX_QUAD * 6];

    int      g_Cnt;

    int ScriptAddQuad()
    {
	MEngine* engine = MEngine::getInstance();
	MScriptContext* script = engine->getScriptContext();
	if(g_Cnt >= MAX_QUAD) return 0;

	MVector3 verts[4];
	MVector2 uvs[4];
	for(int i = 0; i < 4; ++i)
	{
	    MVector2 vert;
	    script->getFloatArray(i*2, &vert.x, 2);
	    verts[i] = MVector3(vert.x, vert.y, 1);
	    script->getFloatArray(i*2 + 1, &uvs[i].x, 2);
	}
	g_Vertices[g_Cnt*6 + 0] = verts[0];
	g_Vertices[g_Cnt*6 + 1] = verts[1];
	g_Vertices[g_Cnt*6 + 2] = verts[2];
	g_Vertices[g_Cnt*6 + 3] = verts[0];
	g_Vertices[g_Cnt*6 + 4] = verts[2];
	g_Vertices[g_Cnt*6 + 5] = verts[3];
	g_UVs[g_Cnt*6 + 0] = uvs[0];
	g_UVs[g_Cnt*6 + 1] = uvs[1];
	g_UVs[g_Cnt*6 + 2] = uvs[2];
	g_UVs[g_Cnt*6 + 3] = uvs[0];
	g_UVs[g_Cnt*6 + 4] = uvs[2];
	g_UVs[g_Cnt*6 + 5] = uvs[3];
	g_Cnt ++;

	return 0;
    }

    void RegisterScript(MScriptContext* script)
    {
	script->addFunction("RendererAddQuad", ScriptAddQuad);

	g_Cnt = 0;
    }

    void Flush()
    {
	if(g_Cnt == 0) return;

	MEngine* engine = MEngine::getInstance();
	MRenderingContext* render = engine->getRenderingContext();
	MSystemContext* system = engine->getSystemContext();
	
	unsigned int w, h;
	system->getScreenSize(&w, &h);
	
	// set up the "camera"
	render->setViewport(0, 0, w, h);
	render->setMatrixMode(M_MATRIX_PROJECTION);
	render->loadIdentity();
	render->setOrthoView(0, w, h, 0, 1.0f, -1.0f);
	render->setMatrixMode(M_MATRIX_MODELVIEW);
	render->loadIdentity();

	// set up arrays
	render->enableVertexArray();
	render->enableTexCoordArray();

	render->setVertexPointer(M_FLOAT, 3, g_Vertices);
	render->setTexCoordPointer(M_FLOAT, 2, g_UVs);

	render->disableCullFace();
	render->disableDepthTest();

	render->drawArray(M_PRIMITIVE_TRIANGLES, 0, g_Cnt * 6);

	g_Cnt = 0;

	g_Selected = NULL;
    }
}
}
