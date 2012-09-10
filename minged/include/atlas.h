#ifndef __MINGED_ATLAS_H__
#define __MINGED_ATLAS_H__

#include <MCore.h>
#include "util.h"

#include <map>

namespace minged
{

class Atlas
{
public:
    Atlas(int maxSize = 1024, int bpp = 4);

    static void RegisterScript(MScriptContext* script);

    bool AddImage(MImage* image, const char* name);
    void Generate(bool clear);
    bool GetUVs(MVector2* uv, const char* image);

    void Select();
private:
    typedef struct _imageDef imageDef;
    typedef std::map<std::string, imageDef> imageMap;
    typedef imageMap::iterator         imageMapIter;

    void WriteImage(imageDef& image);

    int      m_BPP;
    int      m_MaxSize;
    imageMap m_Images;

    uint32   m_Width;
    uint32   m_Height;

    uint32   m_TextureID;
    char*    m_Layout;

    MImage   m_Atlas;
};

};

#endif/*__MINGED_ATLAS_H__*/
