#ifndef __MINGED_ATLAS_H__
#define __MINGED_ATLAS_H__

#include <MCore.h>
#include <MConfigFile.h>

#include "util.h"

#include <map>

#define MINGED_ATLAS_DEFAULT_NAME "atlases/atlas.png"

namespace minged
{

class Atlas
{
public:
    Atlas(int maxSize = 256, int bpp = 4, const char* name = MINGED_ATLAS_DEFAULT_NAME);

    static void RegisterScript(MScriptContext* script);

    MImage* AddImage(MImage* image, const char* name);
    void Generate(bool clear);
    bool GetUVs(MVector2* uv, const char* image);
    MImage* GetImage() { return &m_Atlas; }

    void Select();

    void Save();
private:
    typedef struct _imageDef imageDef;
    typedef std::map<std::string, imageDef> imageMap;
    typedef imageMap::iterator         imageMapIter;

    void WriteImage(imageDef& image);
    std::string m_Name;

    int      m_BPP;
    int      m_MaxSize;
    imageMap m_Images;

    uint32   m_Width;
    uint32   m_Height;

    uint32   m_TextureID;
    char*    m_Layout;

    MImage   m_Atlas;

    MIConfigFile* m_ConfigFile;
    bool     m_Locked;
};

};

#endif/*__MINGED_ATLAS_H__*/
