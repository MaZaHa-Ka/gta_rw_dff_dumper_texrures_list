// MIT лицензия by MAZAHAKA!!
#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "Windows.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdint>
#include <vector>

//#include "tools.h"
#define ROUNDSIZE(x) if((x) & 3) (x) += 4 - ((x)&3)
#define null NULL

// anims :(
//#include "../plugin_III.h"
//#include "AnimBlendFrameData.h"
//#include "CAnimationStyleDescriptor.h"
//#include "CAnimBlendAssocGroup.h"
//#include "CAnimBlendAssociation.h"
//#include "CAnimBlendClumpData.h"
//#include "CAnimBlendHierarchy.h"
//#include "CAnimBlendLink.h"
//#include "CAnimBlendNode.h"
//#include "CAnimBlendSequence.h"
//#include "CAnimBlock.h"
//#include "CAnimManager.h"


//------------------------------------------------------------------
HANDLE InitConsole() // with proto
{
    AllocConsole();

    //SetConsoleOutputCP(866);
    setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);


    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

    return hConsole;
}

//void LeaveConsole(HANDLE hConsole = nullptr)
void LeaveConsole(HANDLE hConsole) // with proto
{
    if (hConsole != nullptr) { SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); } // Reset to default color
    FreeConsole();
}

void SetCurrDir()
{
    char currentDir[MAX_PATH]; // dynamic set curr dir to exe
    GetModuleFileName(NULL, currentDir, MAX_PATH);
    std::string::size_type pos = std::string(currentDir).find_last_of("\\/");
    SetCurrentDirectory(std::string(currentDir).substr(0, pos).c_str());
}




class CAnimBlendHierarchy
{
public:
    char name[24];
    void* sequences;
    int16_t numSequences;
    bool compressed;
    bool keepCompressed;
    float totalLength;
    void* linkPtr;
}; // 40байт

#define NUMANIMBLOCKS 60
#define MAX_ANIMBLOCK_NAME 20

struct CAnimBlock
{
    char name[MAX_ANIMBLOCK_NAME];
    bool isLoaded;
    int16_t refCount;
    int32_t firstIndex;	// first animtion in ms_aAnimations
    int32_t numAnims;
};

struct IfpHeader {
    char ident[4];
    uint32_t size;
};

void LoadIFP(const char* path)
{
    IfpHeader anpk, info, name, dgan, cpan, anim;
#define ROUNDSIZE(x) if((x) & 3) (x) += 4 - ((x)&3)
    std::vector<CAnimBlendHierarchy> animations;
    //CAnimBlock ms_aAnimBlocks[NUMANIMBLOCKS];
    CAnimBlock ped_block;
    int dummy = 0;


    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) { std::cout << "Error opening file: " << path << std::endl; return; }

    file.read((char*)(&anpk), sizeof(anpk));
    ROUNDSIZE(anpk.size);

    file.read((char*)(&info), sizeof(info));
    ROUNDSIZE(info.size);


    //----some info AnimBlock
    //uint32_t numAnimations;
    file.read((char*)(&ped_block.numAnims), sizeof(ped_block.numAnims));

    char block_name[4]; // ped\0
    file.read((char*)(&block_name), sizeof(block_name));
    strncpy(ped_block.name, block_name, MAX_ANIMBLOCK_NAME);

    ped_block.firstIndex = 0; // ms_numAnimations debug 0
    ped_block.isLoaded = true;
    //---------------

    //animations.resize(ped_block.numAnims);
    for (int j = 0; j < ped_block.numAnims; j++)
    {
        CAnimBlendHierarchy* her = new CAnimBlendHierarchy();

        // animation name
        file.read((char*)(&name), sizeof(IfpHeader)); // name tmp перечисляем
        ROUNDSIZE(name.size);
        file.read((char*)(&her->name), name.size);
        //strncpy(this->name, name, 24); // CAnimBlendHierarchy::SetName(char* name)

        file.read((char*)(&dgan), sizeof(IfpHeader));
        ROUNDSIZE(dgan.size);
        file.read((char*)(&info), sizeof(IfpHeader));
        ROUNDSIZE(info.size);
        //file.read((char*)(&her->numSequences), sizeof(her->numSequences)); //22
        file.read((char*)(&her->numSequences), sizeof(int)); //22 pad2
        //her->numSequences = *(int*)buf;
        file.read((char*)(&dummy), sizeof(dummy)); //42 unk
        //file.seekg(her->numSequences * (100 + 8), std::ios::cur); // 100 anim+kr00 не все 100
        //continue;

        for (int k = 0; k < her->numSequences; k++) {
            file.read((char*)(&cpan), sizeof(IfpHeader));
            ROUNDSIZE(cpan.size); // dgan?
            file.seekg(cpan.size, std::ios::cur); // 100 anim+kr00
            //std::cout << "cpan sz: " << cpan.size << "\n";
            continue;

            file.read((char*)(&anim), sizeof(IfpHeader));
            ROUNDSIZE(anim.size);
            //RwStreamRead(stream, buf, anim.size);
            //int numFrames = *(int*)(buf + 28);
            file.seekg(anim.size, std::ios::cur); // 44
        }

        //VALIDATE_SIZE(CAnimBlendSequence, 0x30);
        //file.seekg(her->numSequences * 0x30, std::ios::cur); // 24 уже считали


        animations.push_back(*her);
    }

    //file.seekg(16, std::ios::cur);
    //file.seekg(sizeof(IFPHeader), std::ios::beg);
    //file.read(reinterpret_cast<char*>(&numAnimations), sizeof(numAnimations));

    for (int i = 0; i < animations.size(); i++)
    {
        std::cout << animations[i].name << "\n";
    }
}


void func()
{
    const char* ifp = "D:\\games\\BONUS\\IFP\\anims\\anim_vc\\ped.ifp";
    LoadIFP(ifp);
}





enum RwCorePluginID
{
    rwID_NAOBJECT = 0x00,
    rwID_STRUCT = 0x01,
    rwID_STRING = 0x02,
    rwID_EXTENSION = 0x03,
    rwID_CAMERA = 0x05,
    rwID_TEXTURE = 0x06,
    rwID_MATERIAL = 0x07,
    rwID_MATLIST = 0x08,
    rwID_ATOMICSECT = 0x09,
    rwID_PLANESECT = 0x0A,
    rwID_WORLD = 0x0B,
    rwID_SPLINE = 0x0C,
    rwID_MATRIX = 0x0D,
    rwID_FRAMELIST = 0x0E,
    rwID_GEOMETRY = 0x0F,
    rwID_CLUMP = 0x10,
    rwID_LIGHT = 0x12,
    rwID_UNICODESTRING = 0x13,
    rwID_ATOMIC = 0x14,
    rwID_TEXTURENATIVE = 0x15,
    rwID_TEXDICTIONARY = 0x16,
    rwID_ANIMDATABASE = 0x17,
    rwID_IMAGE = 0x18,
    rwID_SKINANIMATION = 0x19,
    rwID_GEOMETRYLIST = 0x1A,
    rwID_HANIMANIMATION = 0x1B,
    rwID_TEAM = 0x1C,
    rwID_CROWD = 0x1D,
    rwID_DMORPHANIMATION = 0x1E,
    rwID_RIGHTTORENDER = 0x1f,
    rwID_MTEFFECTNATIVE = 0x20,
    rwID_MTEFFECTDICT = 0x21,
    rwID_TEAMDICTIONARY = 0x22,
    rwID_PITEXDICTIONARY = 0x23,
    rwID_TOC = 0x24,
    rwID_PRTSTDGLOBALDATA = 0x25,
    /* Insert before MAX and increment MAX */
    rwID_COREPLUGINIDMAX = 0x26,
};
enum Flags
{
    TRISTRIP = 0x01,
    POSITIONS = 0x02,
    TEXTURED = 0x04,
    PRELIT = 0x08,
    NORMALS = 0x10,
    LIGHT = 0x20,
    MODULATE = 0x40,
    TEXTURED2 = 0x80,
    // When this flag is set the geometry has
    // native geometry. When streamed out this geometry
    // is written out instead of the platform independent data.
    // When streamed in with this flag, the geometry is mostly empty.
    NATIVE = 0x01000000,
    // Just for documentation: RW sets this flag
    // to prevent rendering when executing a pipeline,
    // so only instancing will occur.
    // librw's pipelines are different so it's unused here.
    NATIVEINSTANCE = 0x02000000
};

struct RGBA
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};
struct ChunkHeaderInfo // rwbase.h bool findChunk()
{
    uint32_t type;
    uint32_t length;
    uint32_t version, build;
};
struct GeoStreamData
{
    uint32_t flags;
    int32_t numTriangles;
    int32_t numVertices;
    int32_t numMorphTargets;
};
struct SurfaceProperties
{
    float ambient;
    float specular;
    float diffuse;
};
struct MatStreamData
{
    int32_t flags;	// unused according to RW
    RGBA  color;
    int32_t unused;
    int32_t textured;
};


//--rwbase.h
inline int
libraryIDUnpackVersion(uint32_t libid)
{
    if (libid & 0xFFFF0000)
        return ((libid >> 14 & 0x3FF00) + 0x30000) |
        (libid >> 16 & 0x3F);
    else
        return libid << 8;
}

inline int
libraryIDUnpackBuild(uint32_t libid)
{
    if (libid & 0xFFFF0000)
        return libid & 0xFFFF;
    else
        return 0;
}

// base.cpp
bool
readChunkHeaderInfo(std::ifstream* file, ChunkHeaderInfo* header)
{
    struct {
        int32_t type, size;
        uint32_t id;
    } buf;
    if (!file) { return false; }
    //s->read32(&buf, 12);
    file->read((char*)&buf, sizeof(buf)); // 12

    //if (s->eof()) { return false; }
    if (!file->good()) { return false; }

    if (!header) { return false; }
    header->type = buf.type;
    header->length = buf.size;
    header->version = libraryIDUnpackVersion(buf.id);
    header->build = libraryIDUnpackBuild(buf.id);
    return true;
}


bool
RwStreamFindChunk(std::ifstream* file, uint32_t type, uint32_t* length, uint32_t* version)
{
    ChunkHeaderInfo header;
    if (!file) { return false; }
    while (readChunkHeaderInfo(file, &header)) {
        if (header.type == rwID_NAOBJECT)
            return false;
        if (header.type == type) {
            if (length)
                *length = header.length;
            if (version)
                *version = header.version;
            return true;
        }
       // s->seek(header.length);
        file->seekg(header.length, std::ios::cur);
        //if (!file->good()) { return false; }
    }
    return false;
}



//----------------------------------------------------------------------------------------------
// Clump[0]->GeometryList[0]->Geometry[N]->MaterialList[0]->Material[N]->Texture([N]?[0]!)
//#define DBG_INFO
#define PUSH_NAMES // its lazy method collect tex names after parsing dff. so todo return readers data
// i dont found simple method automatize RwAnalyze Texture List Dumper so..
#ifdef PUSH_NAMES
// matlist
struct data
{
    //uint32_t filterAddressing = 0;
    std::string name = "";
    std::string mask = "";
};
std::vector<data> gvreq;
// out data
std::vector<std::string> gvtexsreq;
#include <unordered_set>
std::vector<std::string> CollectUniqueTextureNames(const std::vector<data>& materials, bool mask = true)
{
    std::unordered_set<std::string> uniqueTextures;
    /*for (const auto& mat : materials) {
        // Добавляем имя текстуры в множество, если не пустое
        if (!mat.name.empty() && uniqueTextures.find(mat.name) == uniqueTextures.end()) {
            uniqueTextures.insert(mat.name);
        }
    }*/

    for (const auto& mat : materials) {
        // Проверка на уникальность и добавление имени текстуры, если не пустое
        if (!mat.name.empty()) {
            auto result = uniqueTextures.insert(mat.name);
            //if (result.second) {  // Если элемент был добавлен (не был дубликатом)
            //}
        }

        // Проверка на уникальность и добавление имени маски, если не пустое
        if (mask)
        {
            if (!mat.mask.empty()) {
                auto result = uniqueTextures.insert(mat.mask);
                //if (result.second) {  // Если элемент был добавлен (не был дубликатом)
                //}
            }
        }
    }

    // Переносим уникальные имена текстур в вектор для вывода
    return std::vector<std::string>(uniqueTextures.begin(), uniqueTextures.end());
}
#endif

#ifdef DBG_INFO
__declspec(noinline)
#endif
void* ReadPlugin(std::ifstream* file) // PluginList::streamSkip(Stream *stream)
{
    int32_t length;
    ChunkHeaderInfo header;
    if (!file) { return null; }
    if (!RwStreamFindChunk(file, rwID_EXTENSION, (uint32_t*)&length, null)) { return null; }
    while (length > 0)
    {
        if (!readChunkHeaderInfo(file, &header)) { return null; }
        length -= 12;
        file->seekg(header.length, std::ios::cur);
    cont:
        length -= header.length;
    }
}
#ifdef DBG_INFO
__declspec(noinline)
#endif
void* ReadTexture(std::ifstream* file)
{
    uint32_t length = 0;
    uint32_t filterAddressing = 0;
    char name[128], mask[128];
    if (!file) { return null; }
    if (!RwStreamFindChunk(file, rwID_STRUCT, null, null)) { return null; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_STRUCT\n";
#endif
    file->read((char*)&filterAddressing, 4);
    if ((filterAddressing & 0xF000) == 0) { filterAddressing |= (filterAddressing & 0xF00) << 4; } // :/

    if (!RwStreamFindChunk(file, rwID_STRING, &length, null)) { return null; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_STRING\n";
#endif
    file->read(name, length);

    if (!RwStreamFindChunk(file, rwID_STRING, &length, null)) { return null; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_STRING\n";
#endif
    file->read(mask, length);
    ReadPlugin(file);

//-------------------------------------------------------
#ifdef PUSH_NAMES
    gvreq.push_back({ name , mask });
#endif
//-------------------------------------------------------
}

#ifdef DBG_INFO
__declspec(noinline)
#endif
void* ReadMaterial(std::ifstream* file)
{
    uint32_t length, version = 0;
    MatStreamData buf;
    if (!file) { return null; }
    if (!RwStreamFindChunk(file, rwID_STRUCT, null, &version)) { return null; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_STRUCT\n";
#endif
    file->read((char*)&buf, sizeof(buf));
    if (!(version < 0x30400)) { file->seekg(sizeof(SurfaceProperties), std::ios::cur); }
    if(buf.textured)
    {
        if (!RwStreamFindChunk(file, rwID_TEXTURE, null, null)) { return null; }
#ifdef DBG_INFO
        std::cout << "FIND! rwID_TEXTURE\n";
#endif
        void* t = ReadTexture(file);
    }
    ReadPlugin(file);
}

#ifdef DBG_INFO
__declspec(noinline)
#endif
void* ReadMaterialList(std::ifstream* file)
{
    int32_t buff = 0;
    int32_t numMat = 0;
    int32_t* indices = null;
    if (!file) { return null; }
    if (!RwStreamFindChunk(file, rwID_STRUCT, null, null)) { return null; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_STRUCT\n";
#endif
    file->read((char*)&numMat, 4);
    if (numMat == 0) { return null; }
    indices = (int32_t*)malloc(numMat * 4);
    file->read((char*)indices, numMat * 4);

    for(int32_t i = 0; i < numMat; i++)
    {
        if (indices[i] >= 0) {}
        else
        {
            if (!RwStreamFindChunk(file, rwID_MATERIAL, null, null)) { return null; }
#ifdef DBG_INFO
            std::cout << "FIND! rwID_MATERIAL\n";
#endif
            void* m = ReadMaterial(file);
		}
	}
    return null;
}

#ifdef DBG_INFO
__declspec(noinline)
#endif
void* ReadGeometry(std::ifstream* file)
{
    //return null;
    if (!file) { return null; }
    //----uint32_t buf32 = 0;
    uint32_t version = 0;
    if (!RwStreamFindChunk(file, rwID_STRUCT, null, &version)) { return null; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_STRUCT\n";
#endif
    GeoStreamData buf;
    SurfaceProperties surfProps;
    file->read((char*)&buf, sizeof(buf));

    if (version < 0x34000) { file->read((char*)&surfProps, 12); }
    //Geometry* Geometry::create
    uint32_t geo_flags = buf.flags & 0xFF00FFFF;
    int32_t geo_numTexCoordSets = (geo_flags & 0xFF0000) >> 16;
    int32_t geo_numMorphTargets = 1; // geo->addMorphTargets(1); Geometry*  Geometry::create
    if ((buf.numMorphTargets - 1) != 0) { geo_numMorphTargets += (buf.numMorphTargets - 1); } // Geometry::addMorphTargets(int32 n)
    if (geo_numTexCoordSets == 0) { geo_numTexCoordSets = (geo_flags & TEXTURED) ? 1 : (geo_flags & TEXTURED2) ? 2 : 0; }

    //printf("geo_flags: %d\n", geo_flags);
    //printf("geo_numTexCoordSets: %d\n", geo_numTexCoordSets);
    //printf("geo_numMorphTargets: %d\n", geo_numMorphTargets);

    if (!(geo_flags & NATIVE)) {
        if (geo_flags & PRELIT)
        {
            //file->read((char*)&buf32, 4 * buf.numVertices); 
            file->seekg(4 * buf.numVertices, std::ios::cur);
        }
        for (int32_t i = 0; i < geo_numTexCoordSets; i++)
        {
            //file->read((char*)&buf32, 2 * buf.numVertices * 4);
            file->seekg(2 * buf.numVertices * 4, std::ios::cur);
        }
        for (int32_t i = 0; i < buf.numTriangles; i++)
        {
            //file->read((char*)&buf32, 8);
            file->seekg(8, std::ios::cur);
        }
    }

    for (int32_t i = 0; i < geo_numMorphTargets; i++)
    {
        file->seekg(4*4, std::ios::cur);
        int32_t hasVertices = 0;
        int32_t hasNormals = 0;
        file->read((char*)&hasVertices, 4);
        file->read((char*)&hasNormals, 4);

        if (hasVertices)
        {
            file->seekg(3 * buf.numVertices * 4, std::ios::cur);
        }
        if (hasNormals)
        {
            file->seekg(3 * buf.numVertices * 4, std::ios::cur);
        }
    }



    if (!RwStreamFindChunk(file, rwID_MATLIST, null, null)) { return null; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_MATLIST\n";
#endif

    // matlist read
    void* ml = ReadMaterialList(file);

    // plugin read offset
    ReadPlugin(file);
    return null;
}

#ifdef DBG_INFO
__declspec(noinline)
#endif
void LoadDFF(const char* path)
{
    //rwID_CLUMP->rwID_GEOMETRYLIST->rwID_STRUCT->rwID_GEOMETRY->
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) { std::cout << "Error opening file: " << path << std::endl; return; }
    if (!RwStreamFindChunk(&file, rwID_CLUMP, null, null)) { return; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_CLUMP\n";
#endif
    if (!RwStreamFindChunk(&file, rwID_GEOMETRYLIST, null, null)) { return; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_GEOMETRYLIST\n";
#endif
    if (!RwStreamFindChunk(&file, rwID_STRUCT, null, null)) { return; }
#ifdef DBG_INFO
    std::cout << "FIND! rwID_STRUCT\n";
#endif
    int32_t numGeometries = 0;
    file.read((char*)&numGeometries, sizeof(numGeometries)); // clump.cpp Clump* Clump::streamRead(Stream * stream)
#ifdef DBG_INFO
    std::cout << "numGeometries: " << numGeometries << "\n";
#endif
    // alloc
    if (numGeometries) {
        //size_t sz = numGeometries * sizeof(Geometry*);
        //void* p = null;
        //p =  malloc(sz);
        //memset(p, 0, sz);
    }
    for (int32_t i = 0; i < numGeometries; i++) {
        if (!RwStreamFindChunk(&file, rwID_GEOMETRY, null, null)) { break; }
#ifdef DBG_INFO
        std::cout << "FIND! rwID_GEOMETRY [" << i+1 << "]\n";
#endif
        void* geo = ReadGeometry(&file);
    }
    //file.read((char*)(&anpk), sizeof(anpk));
    //ROUNDSIZE(anpk.size);
    //file.read((char*)(&dummy), sizeof(dummy)); //42 unk
    //file.seekg(her->numSequences * (100 + 8), std::ios::cur); // 100 anim+kr00 не все 100

#ifdef PUSH_NAMES
#define ALL_

#ifndef ALL
    //http://gta.rockstarvision.com/lcs_files.txt показывает что альфа также в txd мдёт
    bool add_alpha_in_total_list = true; // false без маски, так совпадает с txd от vehicle
    gvtexsreq = CollectUniqueTextureNames(gvreq, add_alpha_in_total_list);
    for (size_t i = 0; i < gvtexsreq.size(); i++)
#else
    for (size_t i = 0; i < gvreq.size(); i++)
#endif
    {
#ifndef ALL
        std::cout << i+1 << ": name: " << gvtexsreq[i] << "\n"; // tex
#else
        std::cout << i+1 << ": name: " << gvreq[i].name << "   mask: " << gvreq[i].mask << "\n"; // data
#endif
    }
#undef ALL
#endif

}

//__declspec(noinline)
void test()
{
    const char* dff = "taxi.dff";
    LoadDFF(dff);
}

int main()
{
    InitConsole(); // 1251
    SetCurrDir();

    //func();
	test();

	return 0;
}
