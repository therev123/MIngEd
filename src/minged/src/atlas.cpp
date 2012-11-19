#include "atlas.h"

#include "renderer.h"

#include <MEngine.h>

minged::Atlas* g_Selected = NULL;

#define PAD 2

namespace minged
{
    int ScriptAtlasCreate()
    {
	MEngine* engine = MEngine::getInstance();
	MScriptContext* script = engine->getScriptContext();

	unsigned int size = script->getInteger(0);
	const char* name = script->getString(1);
	
	script->pushPointer(new Atlas(size, 4, name));
	return 1;
    }

    int ScriptAtlasDestroy()
    {
	MEngine* engine = MEngine::getInstance();
	MScriptContext* script = engine->getScriptContext();

	Atlas* atlas = (Atlas*)script->getPointer(0);
	if(atlas)
	    delete atlas;

	return 0;
    }

    int ScriptAtlasAdd()
    {
	MEngine* engine = MEngine::getInstance();
	MScriptContext* script = engine->getScriptContext();

	Atlas* atlas = (Atlas*)script->getPointer(0);
	const char* path = script->getString(1);

	MImage* img = new MImage;
	if( engine->getImageLoader()->loadData(path, img) && atlas)
	{
	    MImage* ref = atlas->AddImage(img, path);
	    script->pushPointer(ref);
	    if(ref != img)
		delete img;
	    return ref != NULL ? 1 : 0;
	}

	delete img;
	return 0;
    }

    int ScriptAtlasImageDestroy()
    {
	MEngine* engine = MEngine::getInstance();
	MScriptContext* script = engine->getScriptContext();

	MImage* image = (MImage*)script->getPointer(0);
	if(image)
	    delete image;

	return 0;
    }
    
    int ScriptAtlasGetSize()
    { 
	MEngine* engine = MEngine::getInstance();
	MScriptContext* script = engine->getScriptContext();
	Atlas* atlas = (Atlas*)script->getPointer(0);
	if(atlas)
	{
	    MImage* image = atlas->GetImage();
	    script->pushInteger(image->getWidth());
	    script->pushInteger(image->getHeight());
	    return 2;
	}
	return 0;
    }

    int ScriptAtlasImageGetSize()
    {
	MEngine* engine = MEngine::getInstance();
	MScriptContext* script = engine->getScriptContext();

	MImage* image = (MImage*)script->getPointer(0);
	if(image)
	{
	    script->pushInteger(image->getWidth());
	    script->pushInteger(image->getHeight());
	    return 2;
	}
	return 0;
    }

    int ScriptAtlasImageWrite()
    {
	MEngine* engine = MEngine::getInstance();
	MScriptContext* script = engine->getScriptContext();

	MImage* image = (MImage*)script->getPointer(0);
	const char* file = script->getString(1);

	engine->getImageSaver()->loadData(file, image);
	return 0;
    }

    int ScriptAtlasWrite()
    {
	MEngine* engine = MEngine::getInstance();
	MScriptContext* script = engine->getScriptContext();

	Atlas* atlas = (Atlas*)script->getPointer(0);
	atlas->Save();

	return 0;
    }

    int ScriptAtlasGenerate()
    {
	MEngine* engine = MEngine::getInstance();
	MScriptContext* script = engine->getScriptContext();

	if(Atlas* atlas = (Atlas*)script->getPointer(0))
	    atlas->Generate(true);
	return 0;
    }

    int ScriptAtlasSelect()
    {
	MEngine* engine = MEngine::getInstance();
	MScriptContext* script = engine->getScriptContext();

	if(Atlas* atlas = (Atlas*)script->getPointer(0))
	    atlas->Select();
	return 0;	
    }

    int ScriptAtlasGetUVs()
    {
	MEngine* engine = MEngine::getInstance();
	MScriptContext* script = engine->getScriptContext();

	MVector2 uvs[2];
	if(Atlas* atlas = (Atlas*)script->getPointer(0))
	    atlas->GetUVs(uvs, script->getString(1));

	script->pushFloat(uvs[0].x);
	script->pushFloat(uvs[0].y);
	script->pushFloat(uvs[1].x);
	script->pushFloat(uvs[1].y);
	return 4;
    }

    typedef struct _imageDef
    {
	MVector2 uv;
	MImage*  image;
	uint32   width;
	uint32   height;
    } imageDef;

    Atlas::Atlas(int maxSize, int bpp, const char* name)
    : m_MaxSize(maxSize)
    , m_BPP(bpp)
    , m_Width(0)
    , m_Height(0)
    , m_TextureID(0)
    , m_Layout(NULL)
    , m_Name(name)
    {
	char deffile[0xff];
	char* def = deffile;
	util::ChangeExtension(m_Name.c_str(), "atlas", &def);
	m_ConfigFile = (MIConfigFile*)MEngine::getInstance()->loadFile(deffile);
    }

    void Atlas::RegisterScript(MScriptContext* script)
    {
	script->addFunction("mingedAtlasCreate", ScriptAtlasCreate);
	script->addFunction("mingedAtlasDestroy", ScriptAtlasDestroy);
	script->addFunction("mingedAtlasAdd", ScriptAtlasAdd);
	script->addFunction("mingedAtlasGenerate", ScriptAtlasGenerate);
	script->addFunction("mingedAtlasSelect", ScriptAtlasSelect);
	script->addFunction("mingedAtlasGetUVs", ScriptAtlasGetUVs);
	script->addFunction("mingedAtlasWrite", ScriptAtlasWrite);
	script->addFunction("mingedAtlasGetSize", ScriptAtlasGetSize);
	script->addFunction("mingedImageDestroy", ScriptAtlasImageDestroy);
	script->addFunction("mingedImageGetSize", ScriptAtlasImageGetSize);
	script->addFunction("mingedImageWrite", ScriptAtlasImageWrite);
    }

    MImage* Atlas::AddImage(MImage* image, const char* name)
    {
	if(m_Images.find(name) != m_Images.end())
	    return m_Images[name].image;
	

        if(m_Layout == NULL)
	{
	    m_Layout = new char[m_MaxSize * m_MaxSize]; 
	    memset(m_Layout, 0, m_MaxSize * m_MaxSize);
        }
	
	uint32 w = image->getWidth();
	uint32 h = image->getHeight();

	uint32 i = 0;

	MVector2 uv;
	
	// loop fail condition - end of the array 
	uint32 fail = m_MaxSize * m_MaxSize;
	bool found = false;
	while(!found)
	{
	    // look for the next empty pixel
	    while(m_Layout[i] && i < fail) ++i;
	    if(i == fail)
		return NULL;

	    uv.x = i / m_MaxSize;
	    uv.y = i % m_MaxSize;

	    // check for every pixel in this area
	    found = true; // be optimistic
	    for(uint32 x = 0; x < w + PAD; ++x)
		for(uint32 y = 0; y < h + PAD; ++y)
		    if(m_Layout[(m_MaxSize * ((uint32)uv.y+y)) + (uint32)uv.x+x])
			found = NULL; // :(
	    ++i;
	}

	// update the layout to block the space for future
	for(uint32 x = 0 ; x < w + PAD; ++x)
	    for(uint32 y = PAD; y < h + PAD;++y)
		m_Layout[(m_MaxSize * ((uint32)uv.y+y)) + (uint32)uv.x+x] = 1;

	// update the target height
	for(uint32 x = 0; x < m_MaxSize; ++x)
	    for(uint32 y = 0; y < m_MaxSize; ++y)
		if(m_Layout[y*m_MaxSize +x])
		    m_Height = MAX(m_Height, y+1);

	// update the target width
	for(uint32 y = 0; y < m_MaxSize; ++y)
	    for(uint32 x = 0; x < m_MaxSize; ++x)
		if(m_Layout[y*m_MaxSize +x])
		    m_Width = MAX(m_Width, x+1);

	imageDef& def =  m_Images[name];
	def.uv = uv + MVector2(PAD/2, PAD/2);
	def.image = image;
	def.width = image->getWidth();
	def.height = image->getHeight();

	return image;
    }

    void Atlas::Generate(bool clear)
    {
	m_Atlas.create(M_UBYTE, m_Width, m_Height, m_BPP);
	unsigned char col[]= {255, 255, 255, 0};
	for(uint32 x = 0; x < m_Width; ++x)
	    for(uint32 y = 0; y < m_Height; ++y) 
		m_Atlas.writePixel(x, y, col);

	for(imageMapIter iImage = m_Images.begin(); 
	    iImage != m_Images.end(); 
	    iImage++)
	    WriteImage(iImage->second);

	MEngine* engine = MEngine::getInstance();
	MRenderingContext* render = engine->getRenderingContext();
	
	if(m_TextureID == 0)
	    render->createTexture(&m_TextureID);

	render->bindTexture(m_TextureID);
	render->sendTextureImage(&m_Atlas, false, 1, 0);

	if(clear)
	{
	    // remove images
            delete [] m_Layout;
            m_Layout = NULL;
        }
    }

    void Atlas::Select()
    {
	if(g_Selected == this)
	    return;
	MEngine* engine = MEngine::getInstance();
	MRenderingContext* render = engine->getRenderingContext();
	
	Renderer::Flush();

	render->enableTexture();
	render->setBlendingMode(M_BLENDING_ALPHA);
	render->enableBlending();
	
	if(m_TextureID != 0)
	{
	    render->bindTexture(m_TextureID);
	    //render->sendTextureImage(&m_Atlas, false, 1, 0);
	}
	g_Selected = this;
    }

    bool Atlas::GetUVs(MVector2* uv, const char* image)
    {
	if(m_Images.find(image) == m_Images.end())
	    return false;

	imageDef& def = m_Images[image];
        uv[0].x = def.uv.x / (float)m_Width;
        uv[0].y = def.uv.y / (float)m_Height;
	uv[1].x = (def.uv.x + (float)def.width) / (float)m_Width;
	uv[1].y = (def.uv.y + (float)def.height) / (float)m_Height;
	return true;
    }

    void Atlas::WriteImage(imageDef& image)
    {
	uint32 _x = image.uv.x;
	uint32 _y = image.uv.y;
	uint32 h = image.image->getHeight();
	uint32 w = image.image->getWidth();
	unsigned char col[]= {255, 255, 255, 255};
	for(uint32 x = 0; x < w; ++x)
	    for(uint32 y = 0; y < h; ++y)
	    {
		image.image->readPixel(x, y, col);
		m_Atlas.writePixel(_x+x, _y+y, col);
	    }

	// got to write padding too
	for(uint32 i = 1; i <= PAD/2; ++i)
	{
	    for(uint32 x = 0; x < w; ++x)
	    {
		image.image->readPixel(x, 0, col);
		m_Atlas.writePixel(_x+x, _y-i, col);
		image.image->readPixel(x, h-1, col);
		m_Atlas.writePixel(_x+x, _y+i+h-1, col);
	    }
	    for(uint32 y = 0; y < h; ++y)
	    {
		image.image->readPixel(0, y, col);
		m_Atlas.writePixel(_x-i, _y+y, col);
		image.image->readPixel(w-1, y, col);
		m_Atlas.writePixel(_x+i+w-1, _y+y, col);
	    }
	}
    }

    void Atlas::Save()
    {
	MEngine* engine = MEngine::getInstance();

	engine->getImageSaver()->loadData(m_Name.c_str(), &m_Atlas);
	if(m_ConfigFile)
	{
	    m_ConfigFile->Write("atlas.images", m_Images.size());
	    int i = 0;
	    for(imageMapIter iImage = m_Images.begin(); iImage != m_Images.end(); iImage++)
	    {
		std::string root = "atlas.image";
		char num[32];
		snprintf(num, 32, "%d.", i);
		root += num;
		++i;

		m_ConfigFile->Write((root + "name").c_str(), iImage->first);
		m_ConfigFile->Write((root + "pos").c_str(), iImage->second.uv);
		m_ConfigFile->Write((root + "w").c_str(), iImage->second.width);
		m_ConfigFile->Write((root + "h").c_str(), iImage->second.height);
	    }
	    m_ConfigFile->Save();
	}
    }
    
};
